#include "putty_client.h"
#include "connector.h"
#include "putty_gen.h"
#include "vtterminal.h"

extern "C" {

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdarg.h>

#define PUTTY_DO_GLOBALS	       /* actually _define_ globals */
#include "putty.h"
#include "terminal.h"
#include "storage.h"
#include "tree234.h"
#include "winsecur.h"
#include "ssh.h"
#include "vt_ex.h"
#include "terminal.h"

#define WM_AGENT_CALLBACK (WM_APP + 4)

#pragma region callback

	WSAEVENT get_netevent(SOCKET skt)
	{
		return Connector::Instance()->GetNetEvent(skt);
	}

	int from_backend(void *frontend_handle, int is_stderr,
		const char *data, int len)
	{
		PuttyClient* pClient = (PuttyClient*)frontend_handle;
		if (nullptr == pClient)
		{
			return 0;
		}

		return pClient->from_backend(is_stderr, data, len);
	}
	void connection_fatal(void *frontend, const char *szFormat, ...)
	{
		PuttyClient* pClient = (PuttyClient*)frontend;
		if (nullptr == pClient)
		{
			return;
		}

		va_list argList;
		va_start(argList, szFormat);

		int len = _vscprintf(szFormat, argList) + 1;
		if (len == 0)
		{
			va_end(argList);
			return;
		}

		char * buffer = new char[len + 1];
		vsprintf_s(buffer, len, szFormat, argList);
		va_end(argList);

		std::string err_msg(buffer, len);
		delete[] buffer;

		pClient->connection_fatal(frontend, err_msg);
	}
	//for ssh auto login
	int get_userpass_input(prompts_t *p, const unsigned char *in, int inlen)
	{
		if (p->n_prompts == 0)
		{
			//ENG-58649 putty code:console_get_userpass_input
			//wincons.c, line=518: for (curr_prompt = 0; curr_prompt < p->n_prompts; curr_prompt++) {
			return 1;
		}

        int res = -1;
        void *frontend = p->frontend;
		PuttyClient* pClient = (PuttyClient*)frontend;
		if (nullptr == pClient)
		{
			return res;
		}

		std::string strPrompt;
		for (int i = 0; i < (int)p->n_prompts; i++)
		{
			strPrompt += p->prompts[i]->prompt;
		}
		if (strcmp(p->name, "SSH login name") == 0)
		{
			pClient->WaitForUserName(strPrompt);
			//can use: conf_set_str(conf, CONF_username, "user");
			std::string str = pClient->GetUserName();
			pClient->SetUserNameInvalid();

			prompt_t *pr = p->prompts[0];
			prompt_ensure_result_size(pr, str.size() * 5 / 4 + 512);
			strcpy(pr->result, str.c_str());

            res = 1;
		}
		else if (strcmp(p->name, "SSH password") == 0 
            || strcmp(p->name, "SSH server authentication") == 0
            || strcmp(p->name, "SSH key passphrase") == 0)
		{
			pClient->WaitForPassword(strPrompt);
			std::string str = pClient->GetPassword();
			pClient->SetPasswordInvalid();

			if (p->prompts)
			{
				prompt_t *pr = p->prompts[0];
				prompt_ensure_result_size(pr, str.size() * 5 / 4 + 512);
				strcpy(pr->result, str.c_str());

				res = 1;
			}
		}

		return res;
	}

	//get 
	void ask_input(void *frontend, const char* msg, char* buff, int nLen)
	{
		PuttyClient* pClient = (PuttyClient*)frontend;
		if (nullptr == pClient || nullptr == pClient->GetNotify())
		{
			return;
		}

		std::string strInput = "y";
		if (pClient->IsConfirmLogin())
		{
			pClient->WaitForInput(msg, emAskInput);

			strInput = pClient->GetInputBuffer();
			pClient->SetInputBuffer("");
		}
		
		int nLenTemp = nLen > strInput.length() ? strInput.length() : nLen;
		memcpy(buff, strInput.c_str(), nLenTemp);
	}

	//output
	void nb_putty_printf(void *frontend, int err_code, const char *msg)
	{
		PuttyClient* pClient = (PuttyClient*)frontend;
		if (nullptr == pClient || nullptr == pClient->GetNotify())
		{
			return;
		}

		pClient->OutputMessage(err_code, msg);
	}

#pragma endregion callback
	
#pragma region putty_client
	PuttyClient::PuttyClient()
		: conf(nullptr)
 		, backhandle(nullptr)
 		, back(nullptr)
		, use_subsystem(false)
		, m_pNotify(nullptr)
		, m_pEventThread(nullptr)
		, m_netEvent(0)
		, m_emStep(emNoSpecial)
		, m_bLogin(false)
		, m_bLoginConfirm(true)
	{
		Init();
		
		m_inputEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	}


	PuttyClient::~PuttyClient()
	{
		CloseHandle(m_inputEvent);

		DisConnect();

		FreeRecvThread();
	}

	bool PuttyClient::InitEnv()
	{
		sk_init();
		if (p_WSAEventSelect == NULL) {
			fprintf(stderr, "Plink requires WinSock 2\n");
			return false;
		}

		Connector::Instance();
		
		return true;
	}

	bool PuttyClient::IsConnected_unsafe()
	{
		if (nullptr == back || nullptr == backhandle)
		{
			return false;
		}

		return back->connected(backhandle);
	}

	bool PuttyClient::Connect(const std::string& strRemoteHost, int nPort /*= 23*/)
	{
		DisConnect();
		FreeRecvThread();
		m_doDisconnect = false;

		conf_set_str(conf, CONF_host, strRemoteHost.c_str());
		conf_set_int(conf, CONF_port, nPort);

		if (!conf_launchable(conf)/* || !(got_host || loaded_session)*/) {
			//usage();
			return false;
		}

		FixupRemoteHost();

		InitConnParams();

		InitLogCtx();

		std::function<void()> func = [this]() {
			StartUpRecvThread();
		};
		Connector::Instance()->Init(this, func);
		
		return true;
	}

	void PuttyClient::GetTermSize(int width, int height)
	{
		width = conf_get_int(conf, CONF_width);
		height = conf_get_int(conf, CONF_height);
	}

	void PuttyClient::SetTermSize(int width, int height)
	{
		conf_set_int(conf, CONF_width, width);
		conf_set_int(conf, CONF_height, height);
	}

	bool PuttyClient::Send_unsafe(const std::string& data)
	{
		if (IsConnected_unsafe())
		{
			noise_ultralight(data.size());
			back->send(backhandle, data.c_str(), data.size());
		}
		else
		{
			return false;
		}

		return true;
	}

	bool PuttyClient::Init()
	{
		dll_hijacking_protection();

		sklist = NULL;
		skcount = sksize = 0;
		/*
		 * Initialise port and protocol to sensible defaults. (These
		 * will be overridden by more or less anything.)
		 */
		default_protocol = PROT_SSH;
		default_port = 22;

        // REMOVE FLAG_STDERR for display output information.
		flags |= FLAG_VERBOSE;
		/*
		 * Process the command line.
		 */
		conf = conf_new();
		do_defaults(NULL, conf);
		loaded_session = FALSE;
		default_protocol = conf_get_int(conf, CONF_protocol);
		default_port = conf_get_int(conf, CONF_port);
		
		{
			/*
			 * Override the default protocol if PLINK_PROTOCOL is set.
			 */
			char *p = getenv("PLINK_PROTOCOL");
			if (p) {
				const Backend *b = backend_from_name(p);
				if (b) {
					default_protocol = b->protocol;
					default_port = b->default_port;
					conf_set_int(conf, CONF_protocol, default_protocol);
					conf_set_int(conf, CONF_port, default_port);
				}
			}
		}

        return true;
	}

	void PuttyClient::FixupRemoteHost()
	{
		/*
		 * Muck about with the hostname in various ways.
		 */
		{
			char *hostbuf = dupstr(conf_get_str(conf, CONF_host));
			char *host = hostbuf;
			char *p, *q;

			/*
			 * Trim leading whitespace.
			 */
			host += strspn(host, " \t");

			/*
			 * See if host is of the form user@host, and separate out
			 * the username if so.
			 */
			if (host[0] != '\0') {
				char *atsign = strrchr(host, '@');
				if (atsign) {
					*atsign = '\0';
					conf_set_str(conf, CONF_username, host);
					host = atsign + 1;
				}
			}

			/*
			 * Trim a colon suffix off the hostname if it's there. In
			 * order to protect unbracketed IPv6 address literals
			 * against this treatment, we do not do this if there's
			 * _more_ than one colon.
			 */
			{
				char *c = host_strchr(host, ':');

				if (c) {
					char *d = host_strchr(c + 1, ':');
					if (!d)
						*c = '\0';
				}
			}

			/*
			 * Remove any remaining whitespace.
			 */
			p = hostbuf;
			q = host;
			while (*q) {
				if (*q != ' ' && *q != '\t')
					*p++ = *q;
				q++;
			}
			*p = '\0';

			conf_set_str(conf, CONF_host, hostbuf);
			sfree(hostbuf);
		}

	}


	bool PuttyClient::InitConnParams()
	{
		/*
		 * Perform command-line overrides on session configuration.
		 */
		cmdline_run_saved(conf);

		/*
		 * Apply subsystem status.
		 */
		if (use_subsystem)
			conf_set_int(conf, CONF_ssh_subsys, TRUE);

		if (!*conf_get_str(conf, CONF_remote_cmd) &&
			!*conf_get_str(conf, CONF_remote_cmd2) &&
			!*conf_get_str(conf, CONF_ssh_nc_host))
		{
			flags |= FLAG_INTERACTIVE;
		}

		/*
		 * Select protocol. This is farmed out into a table in a
		 * separate file to enable an ssh-free variant.
		 */
		back = backend_from_proto(conf_get_int(conf, CONF_protocol));
		if (back == NULL)
		{
			fprintf(stderr,
				"Internal fault: Unsupported protocol found\n");
			return false;
		}

		/*
		 * Plink doesn't provide any way to add forwardings after the
		 * connection is set up, so if there are none now, we can safely set
		 * the "simple" flag.
		 */
		if (conf_get_int(conf, CONF_protocol) == PROT_SSH &&
			!conf_get_int(conf, CONF_x11_forward) &&
			!conf_get_int(conf, CONF_agentfwd) &&
			!conf_get_str_nthstrkey(conf, CONF_portfwd, 0))
		{
			conf_set_int(conf, CONF_ssh_simple, TRUE);
		}

		return true;
	}

	void PuttyClient::InitLogCtx()
	{
		logctx = log_init(NULL, conf);
		console_provide_logctx(logctx);

		if (restricted_acl)
		{
			logevent(NULL, "Running with restricted process ACL");
		}
	}

	bool PuttyClient::StartUpConn()
	{
		/*
		 * Start up the connection.
		 */
		{
			const char *error;
			char *realhost;
			/* nodelay is only useful if stdin is a character device (console) */
			int nodelay = conf_get_int(conf, CONF_tcp_nodelay) &&
				(GetFileType(GetStdHandle(STD_INPUT_HANDLE)) == FILE_TYPE_CHAR);

			error = back->init(this, &backhandle, conf,
				conf_get_str(conf, CONF_host),
				conf_get_int(conf, CONF_port),
				&realhost, nodelay,
				conf_get_int(conf, CONF_tcp_keepalives));
			if (error) {
				fprintf(stderr, "Unable to open connection:\n%s", error);
				return false;
			}
			back->provide_logctx(backhandle, logctx);
			sfree(realhost);
		}

		return true;
	}

	void PuttyClient::CleanConn()
	{
		if (nullptr == back || nullptr == backhandle)
		{
			return;
		}
		int exitcode = back->exitcode(backhandle);
		if (exitcode < 0) {
			fprintf(stderr, "Remote process exit code unavailable\n");
			exitcode = 1;		       /* this is an error condition */
		}

		back->free(backhandle);
		backhandle = nullptr;

		Connector::Instance()->CloseSocket(m_socket);

		return;
	}

	bool PuttyClient::DoEvent()
	{
		NotifyRecvData();
		
		if (!IsConnected_unsafe())
		{
			if (m_pNotify)
			{
				m_pNotify->OnDisconnected(0, "");
			}

			CleanConn();
			return false;
		}

		DWORD ticks = 0;
		if (!toplevel_callback_pending()) {
			//30s
			ticks = 30 * 1000;
		}

		int nhandles = 0;
		int nCustomHandle = 2;
		HANDLE *handles = handle_get_events(&nhandles);
		handles = sresize(handles, nhandles + nCustomHandle, HANDLE);
		handles[nhandles] = m_inputEvent;
		handles[nhandles + 1] = m_netEvent;

		int n = MsgWaitForMultipleObjects(nhandles + nCustomHandle, handles, FALSE, ticks, QS_POSTMESSAGE);
		if (m_doDisconnect)
		{
			CleanConn();
		}
		else
		{
			if ((unsigned)(n - WAIT_OBJECT_0) < (unsigned)nhandles) {
				//do input event
				handle_got_event(handles[n - WAIT_OBJECT_0]);
			}
			else if (n == WAIT_OBJECT_0 + nhandles) {
				DoInputEvent();
			}
			else if (n == WAIT_OBJECT_0 + nhandles + 1) {
				DoRecvData();
			}
			else if (n == WAIT_OBJECT_0 + nhandles + 1) {
				/*MSG msg;
				while (PeekMessage(&msg, INVALID_HANDLE_VALUE,
					WM_AGENT_CALLBACK, WM_AGENT_CALLBACK,
					PM_REMOVE)) {
					struct agent_callback *c = (struct agent_callback *)msg.lParam;
					c->callback(c->callback_ctx, c->data, c->len);
					sfree(c);
				}*/
			}
		}

		run_toplevel_callbacks();

		sfree(handles);

		/*if (sending)
			handle_unthrottle(stdin_handle, back->sendbuffer(backhandle));*/

		return true;
	}

	void PuttyClient::DoRecvData()
	{
		WSANETWORKEVENTS things;
		SOCKET socket;
		extern SOCKET first_socket(int *), next_socket(int *);
		int i, socketstate;

		/*
		 * We must not call select_result() for any socket
		 * until we have finished enumerating within the tree.
		 * This is because select_result() may close the socket
		 * and modify the tree.
		 */
		 /* Count the active sockets. */
		i = 0;
		for (socket = first_socket(&socketstate);
			socket != INVALID_SOCKET;
			socket = next_socket(&socketstate)) i++;

		/* Expand the buffer if necessary. */
		if (i > sksize) {
			sksize = i + 16;
			sklist = sresize(sklist, sksize, SOCKET);
		}

		/* Retrieve the sockets into sklist. */
		skcount = 0;
		for (socket = first_socket(&socketstate);
			socket != INVALID_SOCKET;
			socket = next_socket(&socketstate)) {
			sklist[skcount++] = socket;
		}

		/* Now we're done enumerating; go through the list. */
		for (i = 0; i < skcount; i++) {
			WPARAM wp;
			socket = sklist[i];
			wp = (WPARAM)socket;
			if (!p_WSAEnumNetworkEvents(socket, NULL, &things)) {
				static const struct { int bit, mask; } eventtypes[] = {
					{FD_CONNECT_BIT, FD_CONNECT},
					{FD_READ_BIT, FD_READ},
					{FD_CLOSE_BIT, FD_CLOSE},
					{FD_OOB_BIT, FD_OOB},
					{FD_WRITE_BIT, FD_WRITE},
					{FD_ACCEPT_BIT, FD_ACCEPT},
				};
				int e;

				noise_ultralight(socket);
				noise_ultralight(things.lNetworkEvents);

				for (e = 0; e < lenof(eventtypes); e++)
					if (things.lNetworkEvents & eventtypes[e].mask) {
						LPARAM lp;
						int err = things.iErrorCode[eventtypes[e].bit];
						lp = WSAMAKESELECTREPLY(eventtypes[e].mask, err);
						select_result(wp, lp);
					}
			}
		}
	}

	void PuttyClient::SetUserName(const std::string& strUserName)
	{
		std::unique_lock<std::mutex> lck(m_mutex);
		m_strUserName = strUserName;

		//conf_set_str(conf, CONF_username, strUserName.c_str());

		m_condVar.notify_one();
	}
#pragma endregion putty_client
	

#pragma region putty_gen

	PuttyGen::PuttyGen()
	{
		m_bNeedPass = false;
		m_bKeyExists = false;
		m_bSsh2 = false;

		m_pSsh2key = nullptr;
		m_pRsaKey = nullptr;
		m_pDsskey = nullptr;
		m_pEckey = nullptr;
	}

	PuttyGen::~PuttyGen()
	{
		sfree(m_pSsh2key);
		sfree(m_pRsaKey);
		sfree(m_pDsskey);
		sfree(m_pEckey);
	}

	bool PuttyGen::IsPpkFile(const std::string& filepath)
	{
		Filename *filename = filename_from_str(filepath.c_str());
		int type = key_type(filename);

		return SSH_KEYTYPE_SSH1 == type || SSH_KEYTYPE_SSH2 == type;
	}

	bool PuttyGen::LoadKeyFile(const std::string& filepath, std::string& strErrmsg)
	{
		m_bNeedPass = false;
		m_bKeyExists = false;

		Filename *filename = filename_from_str(filepath.c_str());

		char *passphrase;
		int needs_pass;
		int type, realtype;
		int ret;
		const char *errmsg = NULL;
		char *comment;
		struct RSAKey newkey1;
		struct ssh2_userkey *newkey2 = NULL;

		type = realtype = key_type(filename);
		if (type != SSH_KEYTYPE_SSH1 &&
			type != SSH_KEYTYPE_SSH2 &&
			!import_possible(type)) 
		{
			char *msg = dupprintf("Couldn't load private key (%s)", key_type_to_str(type));
			strErrmsg = msg;
			sfree(msg);
			return false;
		}

		if (type != SSH_KEYTYPE_SSH1 &&
			type != SSH_KEYTYPE_SSH2) {
			realtype = type;
			type = import_target_type(type);
		}

		comment = NULL;
		passphrase = NULL;
		if (realtype == SSH_KEYTYPE_SSH1)
			needs_pass = rsakey_encrypted(filename, &comment);
		else if (realtype == SSH_KEYTYPE_SSH2)
			needs_pass = ssh2_userkey_encrypted(filename, &comment);
		else
			needs_pass = import_encrypted(filename, realtype, &comment);

		//check if need pass, and get the passphrase
		if (needs_pass)
		{
			if (m_strPassphrase.empty())
			{
				strErrmsg = "the passphrase can not be empty.";
				return false;
			}
			m_bNeedPass = true;
			passphrase = dupstr(m_strPassphrase.c_str());
		}
		else
		{
			passphrase = dupstr("");
		}

		//load key
		if (type == SSH_KEYTYPE_SSH1) {
			if (realtype == type)
				ret = loadrsakey(filename, &newkey1, passphrase, &errmsg);
			else
				ret = import_ssh1(filename, realtype, &newkey1,
					passphrase, &errmsg);
		}
		else {
			if (realtype == type)
				newkey2 = ssh2_load_userkey(filename, passphrase, &errmsg);
			else
				newkey2 = import_ssh2(filename, realtype, passphrase, &errmsg);
			if (newkey2 == SSH2_WRONG_PASSPHRASE)
				ret = -1;
			else if (!newkey2)
				ret = 0;
			else
				ret = 1;
		}

		if (comment)
			sfree(comment);

		if (ret == 0 || ret == -1) {
			char *msg = dupprintf("Couldn't load private key (%s)", errmsg);
			strErrmsg = msg;
			sfree(msg);
		}
		else if (ret == 1) 
		{
			/*
			 * Now update the key controls with all the
			 * key data.
			 */
			{
				if (type == SSH_KEYTYPE_SSH1) 
				{
					char buf[128];

					m_bSsh2 = false;
					m_pRsaKey = new RSAKey;
					*m_pRsaKey = newkey1;
					m_strComment = newkey1.comment;

					/*
					 * Set the key fingerprint.
					 */
					rsa_fingerprint(buf, sizeof(buf), &newkey1);
				}
				else 
				{
					char *fp;

					m_bSsh2 = true;
					m_pSsh2key = new ssh2_userkey;
					*m_pSsh2key = *newkey2;	/* structure copy */
					sfree(newkey2);

					m_strComment = m_pSsh2key->comment;
					
					fp = ssh2_fingerprint(m_pSsh2key->alg, m_pSsh2key->data);
					sfree(fp);
				}
			}
			
			m_bKeyExists = true;
		}

		burnstr(passphrase);

		return m_bKeyExists;
	}

	bool PuttyGen::SavePpkFile(const std::string& filepath, std::string& errmsg)
	{
		if (!m_bKeyExists)
		{
			return false;
		}

		const char* filename = filepath.c_str();
		char *passphrase = NULL;
		if (m_bNeedPass && !m_strPassphrase.empty())
		{
			passphrase = (char*)m_strPassphrase.c_str();
		}

		int ret;
		FILE *fp = fopen(filename, "r");

		if (m_bSsh2) 
		{
			Filename *fn = filename_from_str(filename);

			ret = ssh2_save_userkey(fn, m_pSsh2key, passphrase ? passphrase : NULL);
				
			filename_free(fn);
		}
		else 
		{
			Filename *fn = filename_from_str(filename);
			ret = saversakey(fn, m_pRsaKey, passphrase ? passphrase : NULL);
				
			filename_free(fn);
		}
		if (ret <= 0) {
			return false;
		}

		return true;
	}

#pragma endregion putty_gen

#pragma region terminal

	Terminal* GetTerminal(void* handle)
	{
		if (handle == nullptr)
		{
			return nullptr;
		}
		return (Terminal*)(handle);
	}

	CVtTerminal::CVtTerminal()
		: m_term(nullptr)
		, m_pCallback(nullptr)
	{

	}

	CVtTerminal::~CVtTerminal()
	{
		CVtTerminal::UnRegister(this);
		if (m_term)
		{
			term_free(GetTerminal(m_term));
		}
	}

	void CVtTerminal::Init(int rows, int cols)
	{
		//flags = FLAG_VERBOSE | FLAG_INTERACTIVE;
		Terminal * term = NULL;
		struct conf_tag * conf = conf_new();
		do_defaults(NULL, conf);

		//struct miscdata * data = init_misc_data(conf);
		//data->cb = cb;
		struct unicode_data * uc_data = snew(struct unicode_data);
		memset(uc_data, 0, sizeof(struct unicode_data));
		init_ucs(conf, uc_data);

		conf_set_int(conf, CONF_logtype, LGTYP_NONE);
		//conf_set_int(conf, CONF_lfhascr, 1);
		conf_set_int(conf, CONF_height, rows);
		conf_set_int(conf, CONF_width, cols);
		conf_set_int(conf, CONF_savelines, rows);

		/*
		* Initialise the fonts, simultaneously correcting the guesses
		* for font_{width,height}.
		*/
		//init_fonts(0, 0);

	/*
	* Initialise the terminal. (We have to do this _after_
	* creating the window, since the terminal is the first thing
	* which will call schedule_timer(), which will in turn call
	* timer_change_notify() which will expect hwnd to exist.)
	*/
		term = term_init(conf, uc_data, NULL);
#ifdef TODO_PUTTY_LOG
		term_provide_logctx(term, data->logctx);
#endif // TODO_PUTTY_LOG
		term_size(term, conf_get_int(conf, CONF_height),
			conf_get_int(conf, CONF_width),
			conf_get_int(conf, CONF_savelines));

		m_term = term;
		CVtTerminal::Register(this);
	}

	void CVtTerminal::GetCursorPos(int& x, int& y)
	{
		Terminal* pTerm = GetTerminal(m_term);
		if (pTerm)
		{
			x = pTerm->curs.x;
			y = pTerm->curs.y;
		}
	}

	bool CVtTerminal::IsAlternateScreen()
	{
		Terminal* pTerm = GetTerminal(m_term);
		if (nullptr == pTerm)
		{
			return false;
		}

		return pTerm->alt_which;
	}

	void CVtTerminal::GetBuffer(std::vector<VtLine>& buffer)
	{
		Terminal* pTerm = GetTerminal(m_term);
		if (nullptr == pTerm)
		{
			return;
		}

		int rows = pTerm->rows;
		int cols = pTerm->cols;

		for (int i = 0; i < rows; ++i)
		{
			buffer.push_back(VtLine());
			VtLine& vtLine = buffer.back();

			termline *cline = (lineptr2)(pTerm, i, __LINE__);

			GetTermLine(cline, vtLine);
		}
	}

	void CVtTerminal::GetTermLine(void* line, VtLine& vtLine)
	{
		if (nullptr == line)
		{
			return;
		}
		termline* cline = (termline*)line;

		Terminal* pTerm = GetTerminal(m_term);
		if (nullptr == pTerm)
		{
			return;
		}

		std::wstring& wstr = vtLine.str;

		int cols = pTerm->cols;

		for (int j = 0; j < cols; ++j)
		{
			termchar* chars_temp = cline->chars + j;
			unsigned long tattr = chars_temp->attr;
			unsigned long tchar = chars_temp->chr;

			//copy from terminal.c, function do_paint
			//if (j < term->cols-1 && d[1].chr == UCSWIDE)
			if (j < cols - 1 && chars_temp[1].chr == UCSWIDE)
			{
				wchar_t ch = (wchar_t)tchar;
				wstr.push_back(ch);
				++j;

				vtLine.hasWchar = true;
			}
			else
			{
				wchar_t ch = (wchar_t)(char)tchar;
				wstr.push_back(ch);
			}
		}

		vtLine.hasCrLf = HasCrLf(line);
	}

	bool CVtTerminal::HasCrLf(void* line)
	{
		if (nullptr == line)
		{
			return false;
		}

		termline* cline = (termline*)line;
		if (!(cline->lattr & LATTR_WRAPPED))
		{
			return true;
		}

		return false;
	}

	int CVtTerminal::TermData(const char *data, int len)
	{
		int is_stderr = 0;
		term_data(GetTerminal(m_term), is_stderr, data, len);

		return is_stderr;
	}

#pragma endregion terminal

}

PuttyClientNotify* PuttyClient::GetNotify()
{
	return m_pNotify;
}

void PuttyClient::SetNotify(PuttyClientNotify* pNotify)
{
	m_pNotify = pNotify;
}

bool PuttyClient::IsConnected()
{
	return backhandle != nullptr;
}

bool PuttyClient::DisConnect()
{
	m_doDisconnect = true;
	m_bLogin = false;

	SetEvent(m_netEvent);

	m_condVar.notify_one();

	return true;
}

bool PuttyClient::Send(const std::string& data)
{
	if (TryRspSelf(data))
	{
		return true;
	}
	
	std::lock_guard<std::mutex> lck(m_mutex);
	m_msgQueue.push(data);
	
	SetEvent(m_inputEvent);

	return true;
}

void PuttyClient::SetWSAEvent(SOCKET skt, WSAEVENT netevent)
{
	m_socket = skt;
	m_netEvent = netevent;
}

bool PuttyClient::StartUpRecvThread()
{
	if (nullptr != m_pEventThread)
	{
		assert(false);
		return false;
	}

	m_pEventThread = new std::thread(&PuttyClient::InnerThread, this);

	return true;
}

void PuttyClient::FreeRecvThread()
{
	if (nullptr != m_pEventThread)
	{
		if (m_pEventThread->joinable())
		{
			m_pEventThread->join();
		}
		
		delete m_pEventThread;
		m_pEventThread = nullptr;
	}
}

void PuttyClient::InnerThread()
{
	if (m_pNotify)
	{
		m_pNotify->OnConnected();
	}

	while (1)
	{
		if (!DoEvent())
		{
			break;
		}
	}
}

void PuttyClient::DoInputEvent()
{
	std::lock_guard<std::mutex> lck(m_mutex);
	while (!m_msgQueue.empty())
	{
		std::string str = m_msgQueue.front();
		m_msgQueue.pop();

		Send_unsafe(str);
	}
}

void PuttyClient::NotifyRecvData()
{
	//NotifyRecvData and from_backend are call in the same thread. so no need to add lock
	while (!m_recvQueue.empty())
	{
		std::string str = m_recvQueue.front();
		if (m_pNotify)
		{
			m_pNotify->OnReceived(str.c_str(), str.length());
		}

		m_recvQueue.pop();
	}
	
}

bool PuttyClient::TryRspSelf(const std::string& data)
{
	if (m_emStep == emNoSpecial)
	{
		return false;
	}

	bool bFinish = false;;
	
	if (m_emStep == emUserName)
	{
		std::unique_lock<std::mutex> lck(m_mutex);
		bFinish = AppendInput(m_strUserName, data, true);
	}
	else if (m_emStep == emPassword)
	{
		std::unique_lock<std::mutex> lck(m_mutex);
		bFinish = AppendInput(m_strPassword, data, false);
	}
	else if (m_emStep == emAskInput)
	{
		std::unique_lock<std::mutex> lck(m_mutex);
		bFinish = AppendInput(m_strAskInput, data, true);
	}

	if (bFinish)
	{
		m_condVar.notify_one();
	}
	
	return true;
}

bool PuttyClient::AppendInput(std::string& strBuffer, const std::string& strAppend, bool bDisplay)
{
	bool bFinish = false;
	std::string strDisplay;

	for (size_t i = 0; i < strAppend.size(); ++i)
	{
		char ch = strAppend.at(i);
		if (ch == '\r')
		{
			bFinish = true;
			break;
		}
		
		if (ch == '\b' || ch == 0x7F)
		{
			if (!strBuffer.empty())
			{
				strBuffer.pop_back();
				strDisplay = "\b \b";
			}
			continue;
		}

		strDisplay.push_back(ch);
		strBuffer.push_back(ch);
	}

	if (bDisplay)
	{
		if (m_pNotify)
		{
			m_pNotify->OnReceived(strDisplay.c_str(), strDisplay.length());
		}
	}

	return bFinish;
}

std::string PuttyClient::GetUserName()
{
	return m_strUserName;
}

std::string PuttyClient::GetPassword()
{
	return m_strPassword;
}

void PuttyClient::SetPassword(const std::string& strPassword)
{
	std::unique_lock<std::mutex> lck(m_mutex);
	m_strPassword = strPassword;

	m_condVar.notify_one();
}

bool PuttyClient::IsConfirmLogin()
{
	return m_bLoginConfirm;
}

void PuttyClient::SetLoginConfirm(bool bConfirm)
{
	m_bLoginConfirm = bConfirm;
}

void PuttyClient::SetUserNameInvalid()
{
	m_strUserNameInvalid = m_strUserName;
	m_strUserName = "";
}

void PuttyClient::SetPasswordInvalid()
{
	m_strPasswordInvalid = m_strPassword;
	m_strPassword = "";
}

std::string PuttyClient::GetInputBuffer()
{
	return m_strAskInput;
}

void PuttyClient::SetInputBuffer(const std::string& str)
{
	m_strAskInput = str;
}

void PuttyClient::WaitForInput(const std::string& strPrompt, EnumSpecialStep emStep)
{
	if (m_pNotify)
	{
		std::string str = NormalizeEOL(strPrompt, "\r\n");
		m_pNotify->OnReceived(str.c_str(), str.length());
	}
	m_emStep = emStep;
	do
	{
		bool bDisplay = false;
		const std::string& strBuffer = GetBuffer_unsafe(m_emStep, bDisplay);

		if (bDisplay && !strBuffer.empty())
		{
			if (m_pNotify)
			{
				m_pNotify->OnReceived(strBuffer.c_str(), strBuffer.length());
			}
			break;
		}

		while (1)
		{
			std::unique_lock<std::mutex> lck(m_mutex);
			if (m_doDisconnect)
			{
				break;
			}
			if (!strBuffer.empty())
			{
				break;
			}

			m_condVar.wait(lck);
		}
	} while (0);
	
	m_emStep = emNoSpecial;

	if (m_pNotify)
	{
		std::string strEnd = "\r\n";
		m_pNotify->OnReceived(strEnd.c_str(), strEnd.length());
	}
}

void PuttyClient::WaitForUserName(const std::string& strPrompt)
{
	WaitForInput(strPrompt, emUserName);
}

void PuttyClient::WaitForPassword(const std::string& strPrompt)
{
	WaitForInput(strPrompt, emPassword);
}

std::string PuttyClient::NormalizeEOL(const std::string &text, const std::string &eof)
{
	std::string normalized;
	normalized.reserve(text.length());

	const char *begin = text.c_str();
	const char *end = begin + text.length();
	const char *current = begin;
	while (current != end)
	{
		char c = *current++;
		if (c == '\r') // mac or dos EOL
		{
			if (*current == '\n') // convert dos EOL
				++current;
			normalized += eof;
		}
		else if (c == '\n') // handle unix EOL & other char
		{
			normalized += eof;
		}
		else
		{
			normalized += c;
		}
	}

	return normalized;
}

void PuttyClient::OutputMessage(int err_code, const std::string& str)
{
	if (m_pNotify)
	{
		std::string strNormalize = NormalizeEOL(str, "\r\n");
		if (err_code != 0)
		{
			m_pNotify->OnError(err_code, strNormalize);
		}
		else
		{
			m_pNotify->OnReceived(strNormalize.c_str(), strNormalize.length());
		}
	}
}

std::string& PuttyClient::GetBuffer_unsafe(EnumSpecialStep emStep, bool& bDisplay)
{
	bDisplay = true;

	if (emUserName == emStep)
	{
		return m_strUserName;
	}
	else if (emPassword == emStep)
	{
		bDisplay = false;
		return m_strPassword;
	}
	else if (emAskInput == emStep)
	{
		return m_strAskInput;
	}
	
	assert(false);

	static std::string s_str = "";
	bDisplay = false;
	return s_str;
}

int PuttyClient::from_backend(int is_stderr, const char *data, int len)
{
	std::string str(data, len);
	m_recvQueue.push(str);

	return 0;
}

void PuttyClient::connection_fatal(void *frontend, const std::string& err_msg)
{
	if (m_pNotify)
	{
		m_pNotify->OnError(-1, err_msg);
	}
}


#pragma once
#include <string>
#include <memory>
#include "lw_putty_dll.h"

typedef enum { RSA, DSA, ECDSA, ED25519 } keytype;

class LWPutty_API PuttyGen
{
public:
	PuttyGen();
	~PuttyGen();

	void SetPassphrase(const std::string& passphrase) { m_strPassphrase = passphrase; }

public:
	bool IsPpkFile(const std::string& filepath);
	bool LoadKeyFile(const std::string& filepath, std::string& strErrmsg);
	bool SavePpkFile(const std::string& filepath, std::string& strErrmsg);

protected:
	std::string m_strPassphrase;

protected:
	bool m_bNeedPass;
	bool m_bKeyExists;
	bool m_bSsh2;

	int collecting_entropy;
	int generation_thread_exists;
	int m_entropy_got, m_entropy_required, m_entropy_size;
	int m_key_bits, m_curve_bits;
	
	keytype m_keytype;
	std::string m_strComment;/* points to key.comment or ssh2key.comment */
	
	unsigned *entropy;

	struct ssh2_userkey* m_pSsh2key;
	struct RSAKey* m_pRsaKey;
	struct dss_key* m_pDsskey;
	struct ec_key* m_pEckey;
};



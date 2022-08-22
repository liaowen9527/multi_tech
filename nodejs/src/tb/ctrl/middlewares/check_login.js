const UrlDirect = require('../url_direct');
var Encrypt = require('../../srvc/encrypt');

module.exports = function(req, res, next){
    var encrypt = new Encrypt();
    if (req.session.login_account == encrypt.encrypt_only(req.session.id)) {  // 判断用户session是否存在  
        next();
        return;
    } 
    
    var urlDirect = new UrlDirect(req.url);
    if (urlDirect.isLoginUrl()) {
        next();
    }
    else {
        //ajax can not support redirect; 
        //flow: ajax -> browser -> server -> 302 -> browser(redirect) -> server -> browser -> ajax callback 
        //res.redirect('/relogin.html');
        var result = {result: false, redirect: "/relogin.html" };
        res.send(result);
    }
}
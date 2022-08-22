class UrlDirect {
    constructor(url) {
        this.url = url;
        this.arrUrl = url.split('/');
    }
    isStaticFile() {

    }
    isLoginUrl() {
        var validUrls = ["/user/login", "/captcha/", "/captcha?", "/verify_captcha/"];
        for (var i = 0; i < validUrls.length; ++i) {
            var utlTemp = validUrls[i];
            if (this.url.indexOf(utlTemp) == 0) {
                return true;
            }
        }
        return false;
    }
}

module.exports = UrlDirect;
var crypto = require('crypto');

class VmoreEncrypt{
    constructor() {
        //var buf = crypto.randomBytes(16);
        var buf = 'qwtipngcl';
        this.secrect_key = buf.toString('hex');
    }
    encrypt_only(src) {
        var signature = crypto.createHmac('sha1', this.secrect_key);
        signature.update(src);

        return signature.digest().toString('base64');
    }
    encrypt(src) {
        var cipher = crypto.createCipher('aes192', this.secrect_key);
        var enc = cipher.update(src, 'utf8', 'hex');
        //enc += cipher.final('hex');
        return enc;
    }
    decrypt(src) {
        var decipher = crypto.createDecipher('aes192', this.secrect_key);
        var dec =  decipher.update(src, 'hex', 'utf8');
        //dec += decipher.final('utf8');
        return dec;
    }
}

module.exports = VmoreEncrypt;
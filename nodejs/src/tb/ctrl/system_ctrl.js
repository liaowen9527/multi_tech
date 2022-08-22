const express = require('express');
const bodyParser = require('body-parser');
const log4js = require('log4js');
var util = require('util');
var svgCaptcha = require('svg-captcha');

const router = express.Router();
// 解析 application/x-www-form-urlencoded
const urlencodedParser = bodyParser.urlencoded({ extended: false });
// 解析 application/json
const jsonParser = bodyParser.json();

router.get('/captcha', jsonParser, function (req, res) {
    var codeConfig = {
        size: 4,// 验证码长度
        ignoreChars: '0o1i', // 验证码字符中排除 0o1i
        noise: 2, // 干扰线条的数量
        height: 44 
    }
    var captcha = svgCaptcha.create(codeConfig);
    if (req.session) {
        req.session.captcha = captcha.text.toLowerCase(); //存session用于验证接口获取文字码
    }

    console.log(req.session.id);

    res.setHeader('Content-Type', 'image/svg+xml');
    res.write(String(captcha.data));
    res.end();
    //res.send(codeData);
})

router.get('/verify_captcha/:code', function (req, res, next) {
    //errcode
    var result = {'result' : true};
    
    if(!req.session.captcha){
        result = {'result' : false, 'errcode': 1, 'errmsg' : 'invalid captcha'};
    }
    else if (req.params.code.toLowerCase() != req.session.captcha) {
        result = {'result' : false, 'errcode': 2, 'errmsg' : 'error captcha'};
    }
    else {
        result = {'result' : true};
    }
    
    res.send(result);
})


module.exports = router;
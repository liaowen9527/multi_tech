const express = require('express');
const bodyParser = require('body-parser');
const log4js = require('log4js');
var util = require('util');

const UserDao = require('../dao/user_dao');
var Encrypt = require('../ctrl/encrypt');

const router = express.Router();
// 解析 application/x-www-form-urlencoded
const urlencodedParser = bodyParser.urlencoded({ extended: false });
// 解析 application/json
const jsonParser = bodyParser.json();

const logger = log4js.getLogger('login');
 

router.post('/login', urlencodedParser, function(req, res) {  
    logger.info(util.format('login, body:%s', JSON.stringify(req.body)));
    var dao = new UserDao();

    var result = {}
    dao.getDetail(req.body.username, req.body.password).then(function(user){
        //var info = util.format('您好, %s, 会员等级是%s', JSON.stringify(req.body), user.vip);
        var limit_type = [];
        if (user.limit_type) {
            limit_type = user.limit_type.split(",");
        }
        result = {
            'result': true, 
            'username': user.username,
            'vip': user.vip,
            'limit_day': user.limit_day,
            'limit_week': user.limit_week,
            'limit_total': user.limit_total,
            'limit_type': limit_type
        }

        var encrypt = new Encrypt();
        req.session.username = user.username;
        req.session.login_account = encrypt.encrypt_only(req.session.id);
        res.send(result);
    }, function(err){
        result = {'result': false, 'reason': "用户名或密码错误！"}
        res.send(result);
    });
}) 

router.post('/logout', function(req, res, next){
    req.session.destroy(function(err) {
        if(err){
            res.json({ret_code: 2, ret_msg: '退出登录失败'});
            return;
        }
        
        //res.clearCookie(identityKey);
        //res.redirect('/');
        var result = {result: true, redirect: "/login.html" };
        res.send(result);
    });
});


module.exports = router;
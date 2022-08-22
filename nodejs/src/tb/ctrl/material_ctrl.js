const express = require('express');
const bodyParser = require('body-parser');
const log4js = require('log4js');
var util = require('util');
var moment = require('moment');

const Purchase = require('../model/purchase');

const MaterialDao = require('../dao/material_dao');
const PurchaseDao = require('../dao/purchase_dao');
const UserPermission = require('../services/user_permission_srvc');

const router = express.Router();
// 解析 application/x-www-form-urlencoded
const urlencodedParser = bodyParser.urlencoded({ extended: false });
// 解析 application/json
const jsonParser = bodyParser.json();

const logger = log4js.getLogger('purchase');
 

router.get('/query/:id', urlencodedParser, function(req, res) {  
    logger.info(util.format('login, body:%s', JSON.stringify(req.body)));
    var dao = new MaterialDao();

    var username = req.session.username;
    var result = {}
    dao.get(req.params.id).then(function(material){
        var permission = new UserPermission();
        permission.can_purchase(username, material).then(function(check_result){
            //add purchase record
            var purchase = new Purchase();
            purchase.username = username;
            purchase.material = material._id;
            purchase.time_ = moment().format("YYYY-MM-DD HH:mm:ss");
            var purchaseDao = new PurchaseDao();
            purchaseDao.insert(purchase);

            var limit_count = check_result['limit'];
            limit_count.reduce();

            result['curr_material'] = material;
            result['result'] = true;
            result['limit_count'] = limit_count.tojson();
            res.send(result);
        }, function(err){
            res.send(err);
        });
        
    }, function(err){
        result = {'result': false, 'reason': "没有该素材！"}
        res.send(result);
    });
}) 

router.get('/count', urlencodedParser, function(req, res) { 
    var username = req.session.username;
    var permission = new UserPermission();
    permission.getPurchaseCount(username).then(function(limit_count){
        res.send(limit_count.tojson());
    });
})

router.get('/today_history', urlencodedParser, function(req, res) { 
    //logger.info(util.format('login, body:%s', JSON.stringify(req.body)));
    var dao = new PurchaseDao();

    var result = {};
    var today = new moment();
    dao.history(today).then(function(materialList){
        //var info = util.format('您好, %s, 会员等级是%s', JSON.stringify(req.body), user.vip);
        result['dataList'] = materialList;
        result['result'] = true;
        
        res.send(result);
    }, function(err){
        result = {'result': false, 'reason': "系统故障，请重试，如果多次出现，请联系管理员！"}
        res.send(result);
    });
})


module.exports = router;
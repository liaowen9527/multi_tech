const Q = require('q');
const LimitCount = require('../model/limit_count');

const UserDao = require('../dao/user_dao');
const PurchaseDao = require('../dao/purchase_dao');
//const MaterialDao = require('../dao/material_dao');

class UserPermisionSrvc{
    constructor() {
        //var buf = crypto.randomBytes(16);
        this.userDao = new UserDao();
        this.purchaseDao = new PurchaseDao();
        //this.materialDao = new MaterialDao();
    }
    check_limit_type(user, material) {
        var defer = Q.defer();
        var limit_type = [];
        var bReject = false;
        if (user.limit_type) {
            limit_type = user.limit_type.split(",");
            if (limit_type.length > 0 && limit_type.indexOf(material.type_) == -1) {
                var result = {result: false, reason: '不能购买该类型素材'};
                defer.reject(result);
                bReject = true;
            }
        }
        
        if(!bReject){
            defer.resolve();
        }
        
        return defer.promise;
    }
    check_today_count(user) {
        var defer = Q.defer();
        this.purchaseDao.today_count(user.username).then(function(count){
            if(user.limit_day > 0 && user.limit_day <= count){
                var result = {result: false, reason: '今日购买次数已用完，请明日再购买'};
                defer.reject(result);
            } else {
                var result = {result: true, left: user.limit_day - count};
                defer.resolve(result);
            }
        });

        return defer.promise;
    }
    check_week_count(user) {
        var defer = Q.defer();
        this.purchaseDao.week_count(user.username).then(function(count){
            if(user.limit_week > 0 && user.limit_week <= count){
                var result = {result: false, reason: '本周购买次数已用完，请下周再购买'};
                defer.reject(result);
            } else {
                var result = {result: true, left: user.limit_week - count};
                defer.resolve(result);
            }
        });

        return defer.promise;
    }
    check_total_count(user) {
        var defer = Q.defer();
        this.purchaseDao.week_count(user.username).then(function(count){
            if(user.limit_total > 0 && user.limit_total <= count){
                var result = {result: false, reason: '您的购买次数已用完，请联系卖家'};
                defer.reject(result);
            } else {
                var result = {};
                if (user.limit_total > 0) {
                    result = {result: true, left: user.limit_total - count};
                } else {
                    result = {result: true, left: user.limit_total};
                }
                
                defer.resolve(result);
            }
        });

        return defer.promise;
    }
    can_purchase(username, material) {
        var defer = Q.defer();
        var self = this;
        var limit_count = new LimitCount();
        this.userDao.getDetailByUsername(username).then(function(user){
            self.check_limit_type(user, material).then(function(){
                return self.check_today_count(user);
            }, function(err) {
                throw err;
            }).then(function(left) {
                limit_count.limit_day = left.left;
                return self.check_week_count(user);
            }, function(err) {
                throw err;
            }).then(function(left) {
                limit_count.limit_week = left.left;
                return self.check_total_count(user);
            }, function(err) {
                throw err;
            }).then(function(left) {
                limit_count.limit_total = left.left;
                defer.resolve({result: true, limit: limit_count});
            }, function(err) {
                defer.reject(err);
            }).catch(function(err){
                defer.reject(err);
            });
        }, function(err){
            var result = {'result': false, 'reason': "找不到您的注册信息，请重试，如果一直无法购买，请联系卖家"}
            defer.reject(result);
        });

        return defer.promise;
    }
    getPurchaseCount(username){
        var defer = Q.defer();
        Q.all(
            [this.purchaseDao.today_count(username),
            this.purchaseDao.week_count(username),
            this.purchaseDao.total_count(username)]
        ).spread(function(today_count, week_count, total_count){
            var limit_count = new LimitCount(today_count, week_count, total_count);
            defer.resolve(limit_count);
        });

        return defer.promise;
    }
}


module.exports = UserPermisionSrvc;
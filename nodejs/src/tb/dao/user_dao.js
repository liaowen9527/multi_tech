const util = require('util');
const Q = require('q');
const log4js = require('log4js');
var mysql = require('mysql');

const MysqlConn = require("./mysql_conn");
const User = require("../model/user");

const logger = log4js.getLogger('login');

class UserDao {
    constructor() {
        var mysqlConn = new MysqlConn();
        this.conn = mysqlConn.getConn();
    }

    insert(user){
        var strSql = mysql.format('INSERT INTO USER(USERNAME, PASSWORD, VIP, REGISTER_TIME) \
            values(?, ?, ?, ?)'
            , [user.username, user.password, user.vip, user.register_time]);

        this.conn.query(strSql, function (error, results, fields) {
            if (error) {
                logger.error("UserDao.history");
                logger.error(error);
                defer.reject(error);
                return;
            }
            
            console.log('The solution is: ', results[0].solution);
        });
    }

    get(username, password){
        var defer = Q.defer();
        var strSql = mysql.format('SELECT * FROM USER WHERE USERNAME = ? AND PASSWORD = ?'
            , [username, password]);
        
        this.conn.query(strSql, function (error, results, fields) {
            if (error) {
                logger.error("UserDao.get");
                logger.error(error);
                defer.reject(error);
                return;
            }

            if (results && results.length > 0) {
                defer.resolve(results[0]);
            }
            else {
                defer.reject("username or password is wrong.");
            }
        });

        return defer.promise;
    }
    getDetail(username, password) {
        var defer = Q.defer();
        var strSql = mysql.format('SELECT * FROM USER a inner join vip b on a.vip = b._id WHERE USERNAME = ? AND PASSWORD = ?' 
            , [username, password]);
        
        this.conn.query(strSql, function (error, results, fields) {
            if (error) {
                logger.error("UserDao.get");
                logger.error(error);
                defer.reject(error);
                return;
            }

            if (results && results.length > 0) {
                defer.resolve(results[0]);
            }
            else {
                defer.reject("username or password is wrong.");
            }
        });

        return defer.promise;
    }
    getDetailByUsername(username) {
        var defer = Q.defer();
        var strSql = mysql.format('SELECT * FROM USER a inner join vip b on a.vip = b._id WHERE USERNAME = ?' 
            , [username]);
        
        this.conn.query(strSql, function (error, results, fields) {
            if (error) {
                logger.error("UserDao.get");
                logger.error(error);
                defer.reject(error);
                return;
            }

            if (results && results.length > 0) {
                defer.resolve(results[0]);
            }
            else {
                defer.reject("username or password is wrong.");
            }
        });

        return defer.promise;
    }
}

module.exports = UserDao;
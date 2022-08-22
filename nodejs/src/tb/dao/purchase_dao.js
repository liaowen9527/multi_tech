const util = require('util');
const Q = require('q');
const log4js = require('log4js');
var mysql = require('mysql');
var moment = require('moment');

const MysqlConn = require("./mysql_conn");

const logger = log4js.getLogger('login');

class PurchaseDao {
    constructor() {
        var mysqlConn = new MysqlConn();
        this.conn = mysqlConn.getConn();
    }

    insert(purchase){
        //purchase.time = new Date().Format('yyyy-MM-dd HH:mm:ss');
        var strSql = mysql.format('INSERT INTO purchase(username, material, time_) \
            values(?, ?, ?)'
            , [purchase.username, purchase.material, purchase.time_]);

        this.conn.query(strSql, function (error, results, fields) {
            if (error) {
                logger.error("PurchaseDao.insert");
                logger.error(error);
                defer.reject(error);
                return;
            }
        });
    }

    history(day) {
        var defer = Q.defer();
        var tomorrow = day.clone().add(1, 'days');
        var strSql = mysql.format('select b.baidu_shared FROM purchase a inner join material b on a.material = b._id WHERE a.time_ > ? and a.time_ < ?', 
            [day.format('YYYY-MM-DD'), tomorrow.format('YYYY-MM-DD')]);
        this.conn.query(strSql, function (error, results, fields) {
            if (error) {
                logger.error("PurchaseDao.history");
                logger.error(error);
                defer.reject(error);
                return;
            }

            defer.resolve(results);
        });

        return defer.promise;
    }
    today_count(username) {
        var today = new moment();
        var tomorrow = today.clone().add(1, 'days');

        return this.query_count(username, today.format('YYYY-MM-DD'), tomorrow.format('YYYY-MM-DD'));
    }
    week_count(username) {
        var weekOfday = moment().format('E');
        var monday = moment().subtract(weekOfday - 1, 'days');
        var next_monday = monday.clone().add(7, 'days');

        return this.query_count(username, monday.format('YYYY-MM-DD'), next_monday.format('YYYY-MM-DD'));
    }
    total_count(username) {
        return this.query_count(username);
    }
    query_count(username, begin, end) {
        var defer = Q.defer();
        var strSql = "";
        if (begin && end) {
            strSql = mysql.format('select count(material) as ncount FROM purchase WHERE username = ? and time_ > ? and time_ < ?', 
                [username, begin, end]);
        } else if (begin){
            strSql = mysql.format('select count(material) as ncount FROM purchase WHERE username = ? and time_ > ?', 
                [username, begin]);
        } else if (end){
            strSql = mysql.format('select count(material) as ncount FROM purchase WHERE username = ? and time_ < ?', 
                [username, end]);
        } else {
            strSql = mysql.format('select count(material) as ncount FROM purchase WHERE username = ?', [username]);
        }
        
        this.conn.query(strSql, function (error, results, fields) {
            if (error) {
                logger.error("PurchaseDao.history");
                logger.error(error);
                defer.reject(error);
                return;
            }

            defer.resolve(results[0].ncount);
        });

        return defer.promise;
    }
}

module.exports = PurchaseDao;
const util = require('util');
const Q = require('q');
const log4js = require('log4js');
var mysql = require('mysql');

const MysqlConn = require("./mysql_conn");

const logger = log4js.getLogger('login');

class MaterialDao {
    constructor() {
        var mysqlConn = new MysqlConn();
        this.conn = mysqlConn.getConn();
    }

    insert(material){
        var strSql = mysql.format('INSERT INTO material(_id, num_, type_, \
            baidu_shared, taobao_link, desc_, desc2_) values(?, ?, ?, ?, ?, ?, ?)'
            , [material._id, material.num_, material.type_, 
                material.baidu_shared, material.taobao_link, 
                material.desc_, material.desc2_]);

        this.conn.query(strSql, function (error, results, fields) {
            if (error) {
                logger.error("MaterialDao.insert");
                logger.error(error);
                defer.reject(error);
                return;
            }
            
            console.log('The solution is: ', results[0].solution);
        });
    }

    delete(id){

    }

    update(material){

    }

    get(id){
        var defer = Q.defer();
        var strSql = mysql.format('SELECT * FROM material WHERE _id = ?', [id]);
        this.conn.query(strSql, function (error, results, fields) {
            if (error) {
                logger.error("MaterialDao.get");
                logger.error(error);
                defer.reject(error);
                return;
            }

            if (results && results.length > 0) {
                defer.resolve(results[0]);
            }
            else {
                defer.reject("without the material.");
            }
        });

        return defer.promise;
    }
    getType(id) {
        var defer = Q.defer();
        var strSql = mysql.format('SELECT type_ FROM material WHERE _id = ?', [id]);
        this.conn.query(strSql, function (error, results, fields) {
            if (error) {
                logger.error("MaterialDao.getType");
                logger.error(error);
                defer.reject(error);
                return;
            }

            if (results && results.length > 0) {
                defer.resolve(results[0].type_);
            }
            else {
                defer.reject("without the material.");
            }
        });

        return defer.promise;
    }
}

module.exports = MaterialDao;
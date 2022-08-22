var mysql = require('mysql');

var _instance = null;
class MysqlConn {
    constructor() {
        this.conn = null;
    }

    connect(){
        this.conn = mysql.createConnection({
            host: '182.61.46.195',
            user: 'root',
            password: 'weqeweqe',
            database: 'sucai'
        });
           
        this.conn.connect();
    }

    getConn(){
        if(!this.conn){
            this.connect();
        }

        return this.conn;
    }

    getInstance(){
        if(!_instance){
            _instance = new MysqlConn();
            _instance.connect();
        }

        return _instance;
    }
    
}

module.exports = MysqlConn;
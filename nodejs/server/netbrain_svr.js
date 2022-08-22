const express = require('express');
const session = require('express-session');
const cookieParser = require('cookie-parser');
const log4js = require('log4js');
const util = require("util");


//init log config
const log4j_conf = require('./log4js_conf');
const systemRouter = require('./controller/system_ctrl');
const userRouter = require('./controller/user_ctrl');
const materialRouter = require('./controller/material_ctrl');
const CheckLogin = require('./controller/middlewares/check_login');

const app = express();

app.use(cookieParser());
//must call before use router
app.use(session({
    secret: '12345',
    name: 'testapp',    //这里的name值得是cookie的name，默认cookie的name是：connect.sid
    resave: false,      // 是否每次都重新保存会话，建议false
    saveUninitialized: true,    // 是否自动保存未初始化的会话，建议false
    cookie: {
        maxAge: 10 * 60 * 1000  // 有效期，单位是毫秒， 即10min后session和相应的cookie失效过期
    }
}));


app.use(express.static('html'));
app.use('', systemRouter);
app.use('/user', userRouter);

//只有在下面use的router才会被拦截检测是否login
app.use(CheckLogin);
app.use('/material', materialRouter);


const logger = log4js.getLogger();
 
var server = app.listen(8888, 'localhost', function () {
 
    var host = server.address().address;
    var port = server.address().port;

    var str = util.format("http://%s:%s", host, port);
    console.log(str)
    logger.info(str);
});

console.log("Server has started.");
logger.info("Server has started.");

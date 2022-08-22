var log4js = require('log4js');

log4js.configure({
    appenders: { 
        system: { type: 'file', filename: 'log/vmore.log', backups:0 },
        login: { type: 'file', filename: 'log/login.log', backups:0 },
        purchase: { type: 'file', filename: 'log/purchase.log', backups:0 } 
    },
    categories: { 
        default: { appenders: ['system'], level: 'info' }, 
        login: { appenders: ['login'], level: 'info' }, 
        purchase: { appenders: ['purchase'], level: 'info' }
    }
});

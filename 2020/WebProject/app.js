var express = require('express');
var path = require('path');
var cookieParser = require('cookie-parser');
var logger = require('morgan');

var indexRouter = require('./routes/index');
var usersRouter = require('./routes/users');

var sanitizeHtml = require('sanitize-html');    //Enables Sanitize HTML

var session = require('express-session');       //Enables sessions

var mysql = require('mysql');                   //Adds in mysql

var dbConnectionPool = mysql.createPool({       //Sets up the connection to the SQL database
    host: 'localhost',
    database: 'TestDB'          //The database you wish to use
});

var app = express();

app.use(function (req,res,next) {                //middleware such that the SQL database will be available on all connections
    req.pool = dbConnectionPool;
    next();
});


app.use(logger('dev'));
app.use(express.json());
app.use(express.urlencoded({ extended: false }));
app.use(cookieParser());

app.use(session({                           //Sessions
    secret:'secret_string',
    resave: false,
    saveUninitialized: true,
    cookie: {secure: false, sameSite: 'lax'}
}));


app.use(express.static(path.join(__dirname, 'public')));

app.use('/', indexRouter);
app.use('/users', usersRouter);

module.exports = app;

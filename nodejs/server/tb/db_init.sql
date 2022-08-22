use sucai;
create table user(
	username nvarchar(64) not null primary key,
    password nvarchar(128),
    register_time datetime,
    vip nvarchar(64)
);
insert into user(username, password, vip) values('admin', '!qwecxz123', 'vip');
create table vip(
	_id nvarchar(10) not null primary key,
    limit_day int,
    limit_week int,
    limit_total int,
    limit_type nvarchar(64),
    desc_ nvarchar(128)
);
insert into vip(_id, limit_day, limit_week, limit_total) values('vip', 10, 70, -1);
insert into vip(_id, limit_day, limit_week, limit_total) values('svip', 30, 999, -1);
create table login_history(
	_id nvarchar(64) not null,
	username nvarchar(64) not null,
    begin_time datetime,
    type_ nvarchar(64),
    ip nvarchar(32)
);
create table material(
	_id nvarchar(10) not null primary key,
    num_ int,
    type_ nvarchar(4),
    baidu_shared nvarchar(64),
    taobao_link nvarchar(128),
    desc_ nvarchar(128),
    desc2_ nvarchar(128)
);
create table purchase(
	_id int auto_increment primary key,
	username nvarchar(64) not null,
    material nvarchar(10) not null,
    time_ datetime
);


select count(material) as ncount FROM purchase a WHERE username = 'admin' and a.time_ > '2018-09-03' and a.time_ < '2018-09-04'

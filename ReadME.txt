简易版的MySQL系统，在与下例的空格与断句相同的情况下可实现以下功能（<>为参数名，[]为可选）：
use <database>
create database <name>
create table <name> (<key> <type> [primary], <key> <type>, <key> <type>...)
drop database <name>
drop table <name>
delete <table> where <c> <op> <to>
select <c> from <table> [<c> <op> <to>]
insert <table> values (<const-value>, <const-value>…)（变量数量与数据库列数必须一致）
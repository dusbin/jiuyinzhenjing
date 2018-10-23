/*
不同路径有不同的页面
http://127.0.0.1:8001/list_user
http://127.0.0.1:8001/
http://127.0.0.1:8001/del_user
*/
var express = require('express');
var app = express();
//  主页输出 "Hello World"
app.get('/', function (req, res) {
   console.log("主页 GET 请求");
   res.send('Hello GET');
})
 
 
//  POST 请求
app.post('/', function (req, res) {
   console.log("主页 POST 请求");
   res.send('Hello POST');
})
 
//  /del_user 页面响应
app.get('/del_user', function (req, res) {
   console.log("/del_user 响应 DELETE 请求");
   res.send('删除页面');
})
 
//  /list_user 页面 GET 请求
app.get('/list_user', function (req, res) {
   console.log("/list_user GET 请求");
   res.send('用户列表页面');
})
 
// 对页面 abcd, abxcd, ab123cd, 等响应 GET 请求
app.get('/ab*cd', function(req, res) {   
   console.log("/ab*cd GET 请求");
   res.send('正则匹配');
})
var server = app.listen(8001,function(){
	//var host = server.address().address
	var host = "127.0.0.1"
	var port = server.address().port
	console.log("server running at http://%s:%s/",host,port);
})
/*
var http = require('http');

http.createServer(function (request,response){
	//发送HTTP头部
	//HTTP状态值：200 ok
	//内容类型：text/plain
	response.writeHead(200,{'Content-Type':'text/plain'});
	
	//发送相应数据 “hello world”
	response.end("hello world\n");
	
}).listen(8001);
//打印日志
console.log('server running at http://127.0.0.1:8001/');
*/
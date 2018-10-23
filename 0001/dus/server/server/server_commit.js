var express = require('express');
var app = express();
app.use(express.static('public'));
var net = require('net');
var port = 8000;
var flag_socket = 0;
//get
app.get('/index.htm', function (req,res) {
	console.log("主页 GET 请求");
	res.sendFile(__dirname + "/" + "index.htm");
	//res.send('Hello GET');
})

app.get('/process_get',function(req,res){
	//输出JSON格式
	var response = {
		"serverip":req.query.serverip,
		"sendmsg":req.query.sendmsg
		//"last_name":req.query.last_name
	};
	console.log(response);
	var host = response["serverip"];
	var msg = response["sendmsg"];
	console.log("host begin");
	console.log(host);
	console.log("host end");
	console.log(response);
	var client = new net.Socket();
	client.setEncoding('binary');
	client.connect(port,host,function(){
		client.write(msg);
		flag_socket=1;
	});
	client.on('data',function(data){
		res.end(JSON.stringify(data));
		console.log('from server:'+data);
	});
	client.on('error',function(error){
	//错误出现之后关闭连接
	console.log('error:'+error);
	if(flag_socket == 1){
		client.destory();
	}
	});
	client.on('close',function(){
		//正常关闭连接
		console.log('Connection closed');
	});
	res.end("end with something wrong!");
})
/*
app.post('/process_get',function(req,res){
	//输出JSON格式
	var response = {
		"serverip":req.query.serverip
		//"last_name":req.query.last_name
	};
	var host = response["serverip"];
	console.log("host begin");
	console.log(host);
	console.log("host end");
	console.log(response);
	
	res.end(JSON.stringify(response));
})
*/
var server = app.listen(8001,function(){
	//var host = server.address().address
	var host = "192.168.191.1"
	//var host = "127.0.0.1"
	var port = server.address().port
	console.log("server running at http://%s:%s/",host,port);
})
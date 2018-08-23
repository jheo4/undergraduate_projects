<%@ page language="java" contentType="text/html; charset=EUC-KR"
    pageEncoding="EUC-KR"%>
<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<html>
<head>
<%@page import="com.mongodb.*" %>
<%@page import="com.mongodb.client.*" %>
<%@page import="java.util.*" %>

<meta http-equiv="Content-Type" content="text/html; charset=EUC-KR">
<title>Insert title here</title>
</head>
<body>
<% MongoClient mongoClient=null;

    mongoClient = new MongoClient("localhost",27017);
    System.out.println("connected");
    //WRITE AUTH
    WriteConcern w = new WriteConcern(1,2000);//LOCK NUM, WAIT TIME
    mongoClient.setWriteConcern(w);
    //CONNECT
    DB db = mongoClient.getDB("owhy");
    DBCursor cursor;
    //컬렉션 가져오기
    DBCollection coll = db.getCollection("attack_log");

    Date c = new Date();
    //INSERT
        DBObject doc = new BasicDBObject();
        doc.put("name",request.getParameter("name"));
        doc.put("value",request.getParameter("value"));
        doc.put("time",c);
        WriteResult result=coll.insert(doc);
		
 %><%=result %>
<% %>
</body>
</html>
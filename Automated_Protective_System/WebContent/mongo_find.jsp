<%@ page language="java" contentType="text/html; charset=EUC-KR"
    pageEncoding="EUC-KR"%>
<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=EUC-KR">
<title>mongo_find</title>
<%@page import="com.mongodb.*" %>
<%@page import="com.mongodb.client.*" %>
<%@page import="java.util.*" %>
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
int i=1;
%>
<table style="width:100%; position:absolute; top:0px;left:0px;">
<%
	

    //FIND
    DBObject sortO = new BasicDBObject();
    sortO.put("time",-1);
    
        cursor = coll.find().sort(sortO).limit(15);
        while(cursor.hasNext()){
        	
        		%><tr ><%
        	
        	i++;
        	DBObject temp=cursor.next();
        	String name=(String)temp.get("name");
        	String value=(String)temp.get("value");
        	Date d=(Date)temp.get("time");
           %>
           <td width="30%" ><%=name%></td><td width="20%" ><%=value%></td><td width="50%" ><%=d%></td>
           </tr>
           <%
        }
%>
</table>

<%

%>
</body>
</html>
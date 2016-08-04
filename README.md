# RestClient-MFC
RestClient MFC

# Example

```c
/* 
Params = {
	string: server,
	string: action,
	CHttpConnection: HTTP_VERB_ [ POST, GET, PUT, HEAD, LINK... ],
	string: user=email@example.com&password=231516326,
	int: port
};
*/ 

Rest rest; // Creamos petición rest
// hacemos la petición rest : http://10.63.1.138/BitacoraMovil/Hello
string result = rest.Request("10.63.1.138", "BitacoraMovil/Hello", CHttpConnection::HTTP_VERB_GET, "", 1337);

Rest rest; // Creamos petición rest
// hacemos la petición rest : http://www.myserver.com/ActionUrl/Action?id=15&isactive=true
string result = rest.Request("www.myserver.com", "ActionUrl/Action", CHttpConnection::HTTP_VERB_GET, "id=15&isactive=true", 1337);
```

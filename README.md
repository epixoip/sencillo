# Sencillo

_NOTE: This is still a WIP and not currently ready for use._

Sencillo is a single-header C11 library that adds high-level abstraction and helper functions to Facil.io based webapps using a syntax inspired by Kotlin, including pseudo classes/types and lambda functions.

In addition to what Facil.io already provides, Sencillo adds...

#### Automatic CSRF Protection

#### Advanced route handler with inline parameters:
```
Routes {
    Get("/", ({
        ...
    }));
    
    Post("/profile/:user/:object", ({
       ...
    }));
    
    Put("/uploads/:file", ({
        ...
    }));
    
    Delete("/:foo/:bar/:baz", ({
        ...
    }));
}
```
#### Database pseudo class and generic abstraction (currently only MySQL supported):
Database.connect()  
Database.close()  
Database.prepare()  
Database.bind()  
Database.exec()  
Database.fetchrow()  
Database.column()  

#### Central configuration of both Sencillo and Facil.io via Config pseudo class with JSON config file:
Config.get()  
Config.set()  
Config.load()  

#### Server pseudo class:
Server.start()  
Server.stop()  

#### Session pseudo class & session engine:
Session.create()  
Session.destroy()  
Session.get()  
Session.set()  
Session.getId()  

#### HTTP helper functions and variadic macros:
Header()  
Cookie() / Cookies()  
Method()  
Path()  
Param() / Params()  
Error()  
Redirect()  

#### Simplified Mustache templating:
ModelAndView()  

#### forEach loops with lambdas and implicit _it_ parameters:
```
forEach(myMapObject, ({
    char *key = toString(it);
    printf("key == %s\n", key);
}));
```

#### HashMap pseudo class:
```
Object myMap = mapOf(({
    String("this") to String("that"),
    String("that") to Int(42)
}));

assert(HashMap.contains(myMap, "this"));

HashMap.clear(myMap);

assert(HashMap.isEmpty());
```
mapOf()  
HashMap.create()  
HashMap.keys()  
HashMap.values()  
HashMap.get()  
HashMap.replace()  
HashMap.set()  
HashMap.remove()  
HashMap.contains()  
HashMap.isEmpty()  
HashMap.clear()  
HashMap.count()  

#### List pseudo class:
```
Object myList = listOf((
    String("1st"), 
    Int(2), 
    Float(3.0) 
));

assert(List.size(myList) == 3);
assert(List.contains(myList, Int(2)));
assert(List.indexOf(myList, Float(3.0)) == 2);
```
listOf()  
List.create()  
List.first()  
List.last()  
List.pop()  
List.get()  
List.replace()  
List.remove()  
List.isEmpty()  
List.contains()  
List.append()  
List.size()  
List.indexOf()  

#### String pseudo class:
String.create()  
String.copy()  
String.immutable()  
String.clear()  
String.length()  
String.append()  
String.appendInt()  
String.appendFmt()  
String.fromFile()  

#### Pseudo types and conversions:
String()  
toString()  
Int()  
toInt()  
Float()  
toFloat()  
toJSON()  
fromJSON()  
getClass()  
instanceOf()  

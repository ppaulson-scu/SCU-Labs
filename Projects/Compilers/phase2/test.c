keyword:struct
identifier:node
operator:{
keyword:int
identifier:data
operator:;
keyword:struct
identifier:node
operator:*
identifier:link
operator:;
operator:}
operator:;
keyword:struct
identifier:list
operator:{
keyword:int
identifier:count
operator:;
keyword:struct
identifier:node
operator:*
identifier:head
operator:;
operator:}
operator:;
keyword:struct
identifier:list
identifier:stack
operator:;
keyword:char
operator:*
identifier:malloc
operator:(
keyword:unsigned
keyword:long
identifier:n
operator:)
operator:;
keyword:int
identifier:push
operator:(
keyword:struct
identifier:list
operator:*
identifier:list
operator:,
keyword:int
identifier:x
operator:)
operator:{
keyword:struct
identifier:node
operator:*
identifier:node
operator:;
identifier:node
operator:=
operator:(
keyword:struct
identifier:node
operator:*
operator:)
identifier:malloc
operator:(
keyword:sizeof
operator:(
keyword:struct
identifier:node
operator:)
operator:)
operator:;
identifier:node
operator:->
identifier:link
operator:=
operator:(
keyword:struct
identifier:node
operator:*
operator:)
integer:0
operator:;
operator:(
operator:*
identifier:list
operator:)
operator:.
identifier:head
operator:=
identifier:node
operator:;
identifier:list
operator:->
identifier:head
operator:->
identifier:data
operator:=
operator:--
identifier:x
operator:;
operator:}
keyword:int
identifier:main
operator:(
operator:)
operator:{
identifier:push
operator:(
operator:&
identifier:stack
operator:,
integer:0
operator:)
operator:;
operator:}

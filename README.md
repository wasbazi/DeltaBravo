DeltaBravo - Another Layer of Abstraction
====================

DeltaBravo aims to abstract the Database even further away from the programmer. This means that writing plain SQL commands (or whatever falvor of databases you enjoy) will tend towards extinction, and if we dare, be phased out entirely.

This is a PHP Database Abstraction Layer built on top of the PDO library

Isn't PDO already an Abstraction Layer?
---------------------

PDO already abstracts the user away from the SQL, but only so far. It does come with some fun features such as, preparing SQL statements so the coder doesn't have to worry about whether information given to them is escaped properly.

Moving Away From SQL
---------------------

The main goal is to make interaction with the Database more akin to calling functions on our Database class. Some simple examples are as follows:

`
global $database;
$users = $database->select("users")->execute();
$profile_for_avi = $database->select("profiles")->where("name","Avi","=")->execute();
`

*More examples can be found in the example.php file*
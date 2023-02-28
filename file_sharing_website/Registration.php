<!DOCTYPE html>
<html lang = "en">
<head>
	<title>News Site Login</title>
	<link rel = "stylesheet" type = "text/css" href = "styling.css"/>
</head>
<body>

<?php
session_start();
//php file that handles user registration (checks to see if the username is already in the users table and if so then will say so
//otherwise it will prompt the user to create a password which will then be hashed into the users table (whole things will be 
//added at the end)), will have links to the general news page and then login page
?>

<h1>
New User Registration 
</h1>

<form action="registration_check.php" method="post">
	<h2>
		<label for="Username">New Username:</label>
		<input type="text" name="Username" id="Username" />
    </h2>
    <h2>
		<label for="Password">New Password:</label>
		<input type="text" name="Password" id="Password" />
    </h2>
	<p>
		<input type="submit" value="Send" />
		<input type="reset" />
	</p>
</form>

<form action="Login_page.php">
	<p>
		<input type="submit" value="Login Page" />
	</p>
</form>

</body>
</html>
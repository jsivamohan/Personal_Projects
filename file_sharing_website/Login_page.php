<!DOCTYPE html>
<html lang = "en">
<head>
	<title>News Site Login</title>
	<link rel = "stylesheet" type = "text/css" href = "styling.css"/>
</head>
<?php
session_start();
//php file for the login page (checks to see if user is already registered, if they are then it asks for the password which is
//hashed in the user database and logs them in if it matches), if not then it doesn't work (has links back to the news page or to
//the registration page)
?>

<h1>
News Site Login
</h1>

<form action="new_site_login_check.php" method="post">
<div class  = box>
	<h2>
		<label for="Username">Username:</label>
		<input type="text" name="Username" id="Username" />
    </h2>
    <h2>
		<label for="Password">Password:</label>
		<input type="text" name="Password" id="Password" />
    </h2>
</div>
	<p>
		<input type="submit" value="Send" />
		<input type="reset" />
	</p>
</form>

<form action="Registration.php">
	<p>
		<input type="submit" value="Registration" />
	</p>
</form>

<form action="delete_user.php">
	<p>
		<input type="submit" value="Delete User" />
	</p>
</form>

<form action="News_page.php">
	<p>
		<input type="submit" value="News Page" />
	</p>
</form>

</body>
</html>
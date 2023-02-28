<!DOCTYPE html>
<html lang = "en">
<head>
	<title>User Delete</title>
	<link rel = "stylesheet" type = "text/css" href = "styling.css"/>
</head>
<body>
<?php
//this page is used to delete a user, as people will enter the user's username and passwotd here
session_start();
?>

<h1>
Delete User
</h1>

<form action="delete_user_sql.php" method = "post">
	<h2>
		<label for="Username">Username:</label>
        <input type="hidden" name="token" value="<?php echo $_SESSION['token'];?>" />
		<input type="text" name="Username" id="Username" />
    </h2>
    <h2>
		<label for="Password">Password:</label>
        <input type="hidden" name="token" value="<?php echo $_SESSION['token'];?>" />
		<input type="text" name="Password" id="Password" />
    </h2>
	<p>
		<input type="submit" value="Send" />
		<input type="reset" />
	</p>
</form>

<form action="Login_page.php">
	<p>
		<input type="submit" value="Back to Login" />
	</p>
</form>
</body>
</html>
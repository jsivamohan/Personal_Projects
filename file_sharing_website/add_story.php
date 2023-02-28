<!DOCTYPE html>
<html lang = "en">
<head>
	<title>News Site Login</title>
	<link rel = "stylesheet" type = "text/css" href = "styling.css"/>
</head>
<body>
<?php
session_start();
//script to add a new story to the database
$_SESSION["user_id"];//saved value of the user
?>
<form action="add_story_sql.php" method = "post">
	<h2>
		<label for="Title">Title of Story:</label>
		<input type="hidden" name="token" value="<?php echo $_SESSION['token'];?>" />
		<input type="text" name="Title" id="Title" />
    </h2>
    <h2>
		<label for="Body">Body of Story:</label>
		<input type="hidden" name="token" value="<?php echo $_SESSION['token'];?>" />
		<input type="text" name="Body" id="Body" />
    </h2>
    <h2>
		<label for="Link">Link Story:</label>
		<input type="hidden" name="token" value="<?php echo $_SESSION['token'];?>" />
		<input type="text" name="Link" id="Link" />
    </h2>
	<p>
		<input type="submit" value="Send" />
		<input type="hidden" name="token" value="<?php echo $_SESSION['token'];?>" />
		<input type="reset" />
	</p>
</form>

<form action="News_page.php">
    <p>
		<input type="submit" value="Return to news" />
		<input type="reset" />
	</p>
</form>
</body>
</html>

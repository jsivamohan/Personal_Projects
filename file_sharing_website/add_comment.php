<!DOCTYPE html>
<html lang = "en">

<head>
	<title>News Site Login</title>
	<link rel = "stylesheet" type = "text/css" href = "styling.css"/>
</head>
<body>
<?php
//page for adding a comment and storing in the comment database using the already set session vars for story_id and user_id
session_start();
$_SESSION['story_id'];
$_SESSION['user_id'];

?>

<form action="add_comment_sql.php" method = "post">
	<h2>
		<label for="Comment">Comment:</label>
		<input type="text" name="Comment" id="Comment" />
		<input type="hidden" name="token" value="<?php echo $_SESSION['token'];?>" />
    </h2>
    <p>
		<input type="submit" value="Send" />
		<input type="reset" />
	</p>
</form>
<form action="story_viewing.php">
    <p>
		<input type="submit" value="Return to news" />
		<input type="reset" />
	</p>
</form>

</body>
</html>
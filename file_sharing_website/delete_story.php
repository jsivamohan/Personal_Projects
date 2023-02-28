<!DOCTYPE html>
<html lang = "en">
<head>
	<title>News Site Login</title>
	<link rel = "stylesheet" type = "text/css" href = "styling.css"/>
</head>
<body>
<?php
//this page makes sure that the user wants to delete the story
session_start();
?>
<h2>Are you sure that you want to delete this story?</h2>
	<form action=delete_story_sql.php>
		<p>
			<input type=submit value= 'Yes' />
			<input type="hidden" name="token" value="<?php echo $_SESSION['token'];?>" />
		</p>
	</form>
	<form action=story_viewing>
		<p>
			<input type=submit value= 'No' />
		</p>
	</form>

</body>
</html>
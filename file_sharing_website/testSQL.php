<html>
<!DOCTYPE html>
<?php
echo "hi";
// Content of database.php
$mysqli = new mysqli('localhost', 'wustl_inst', 'wustl_pass', 'mod3Group');

if($mysqli->connect_errno) {
	printf("Connection Failed: %s\n", $mysqli->connect_error);
	exit;
}
?>
</html>
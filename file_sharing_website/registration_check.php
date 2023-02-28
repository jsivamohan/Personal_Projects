<?php
require 'database.php';

session_start();
$_SESSION['New_Username'] = $mysqli->real_escape_string($_POST['Username']);
$_SESSION['New_Password'] = $mysqli->real_escape_string($_POST['Password']);
echo $_SESSION['New_Username'];

//query to check if username in the database, if so then return to the registration page as we don't allow for repeat usernames
//if username not in the database then add an entry to the users database with the username, password, and redirect them to the 
//login page (don't set the user_id session yet as we want them to login in first to set it)

$hashPass = password_hash($_SESSION['New_Password'], PASSWORD_BCRYPT);
echo $hashPass;

$stmt = $mysqli->prepare("select count(username) from users where username like ?");
if(!$stmt){
	printf("Query Prep Failed: %s\n", $mysqli->error);
	exit;
}

$stmt->bind_param('s', $_SESSION['New_Username']);
$stmt->execute();
$stmt->bind_result($num_results);
$stmt->fetch();
$stmt->close();

if($num_results < 1){
    $stmt = $mysqli->prepare("insert into users (username, password) values (?, ?)");
    if(!$stmt){
        printf("Query Prep Failed: %s\n", $mysqli->error);
        exit;
    }

    $stmt->bind_param('ss', $_SESSION['New_Username'], $hashPass);
    $stmt->execute();
    $stmt->close();
    header("Location: Login_page.php");
    exit; 
    }
    header("Location: Registration.php");
    exit; 


?>

<?php
$strange = $_GET['force'];

$servername = "localhost";
$username = "rongkham";
$password = "q3Ny2is51M";
$dbname = "rongkham_iot";

// Create connection
$conn = new mysqli($servername, $username,$password, $dbname);
// Check connection
if ($conn->connect_error) {
    die("Connection failed: " . $conn->connect_error);
} 
//$val = $_GET['temp'];
$sql = "INSERT INTO esp8266(strange) VALUES ('$strange');";

if ($conn->query($sql) === TRUE) {
    echo "save OK";
} else {
    echo "Error:" . $sql . "<br>" . $conn->error;
}

$conn->close();
?>
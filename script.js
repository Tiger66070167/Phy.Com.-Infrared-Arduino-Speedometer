fetch("SpeedOmeterSrccode/SpeedOmeterSrccode.ino")
.then(response => response.text())
.then(data => {
    document.getElementById("codeDisplay").textContent = data;
})
.catch(error => {
    document.getElementById("codeDisplay").textContent = "Error loading file.";
    console.error("Error fetching file:", error);
});
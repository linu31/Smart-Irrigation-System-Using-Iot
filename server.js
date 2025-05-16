const express = require("express");
const cors = require("cors");

const app = express();
app.use(cors());
app.use(express.static("public"));

let latestSensorData = {
  temp: null,
  humidity: null,
  soil: null,
  time: null
};

app.get("/sensordata", (req, res) => {
  const { temp, humidity, soil } = req.query;
  if (temp && humidity && soil) {
    latestSensorData = {
      temp,
      humidity,
      soil,
      time: new Date().toLocaleTimeString()
    };
    console.log("✅ Received:", latestSensorData);
    res.send("Data received!");
  } else {
    res.status(400).send("Missing parameters");
  }
});

app.get("/latest", (req, res) => {
  res.json(latestSensorData);
});

const PORT = 3000;
app.listen(PORT, () => {
  console.log(`🚀 Server running on http://localhost:${PORT}`);
});
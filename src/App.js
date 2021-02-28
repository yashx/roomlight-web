import { Grid, Box, makeStyles } from "@material-ui/core";
import React, { useState } from "react";
import RoomLightButton from "./components/RoomLightButton";
import useInterval from "./hooks/useInterval";

const useStyle = makeStyles((theme) => ({
  container: {
    minHeight: "100%",
    display: "flex",
    flexDirection: "column",
    justifyContent: "center",
  },
  gridContainer: {
    width: "100%",
    margin: "0",
    justifyContent: "center",
    alignItems: "stretch",
  },
}));

function App() {
  const [isLeftWhiteLightOn, setLeftWhiteLightOn] = useState(false);
  const [isRightWhiteLightOn, setRightWhiteLightOn] = useState(false);
  const [isYellowLightOn, setYellowLightOn] = useState(false);
  const [isFanOn, setFanOn] = useState(false);
  const [isPowerOn, setPowerOn] = useState(true);

  const toggle = (tag, currentValue) => {
    const jBody = {};
    jBody[tag] = Number(!currentValue);

    console.log(jBody);

    fetch("/state", {
      method: "POST",
      headers: {
        "Content-Type": "application/json",
      },
      body: JSON.stringify(jBody),
    });
  };

  useInterval(() => {
    fetch("/state")
      .then((res) => res.json())
      .then((j) => {
        setLeftWhiteLightOn(!!j["wLOS"]);
        setRightWhiteLightOn(!!j["wLTS"]);
        setYellowLightOn(!!j["yLS"]);
        setFanOn(!!j["fS"]);
        setPowerOn(!!j["aS"]);
      });
  }, 300);

  const classes = useStyle();

  return (
    <Box className={classes.container}>
      <Grid container className={classes.gridContainer} spacing={1}>
        <RoomLightButton
          text={"Left White Light"}
          isOn={isLeftWhiteLightOn}
          onClick={() => toggle("wLOS", isLeftWhiteLightOn)}
          type={"light"}
        />
        <RoomLightButton
          text={"Right White Light"}
          isOn={isRightWhiteLightOn}
          onClick={() => toggle("wLTS", isRightWhiteLightOn)}
          type={"light"}
        />
        <RoomLightButton
          text={"Yellow Light"}
          isOn={isYellowLightOn}
          onClick={() => toggle("yLS", isYellowLightOn)}
          type={"light"}
        />
        <RoomLightButton
          text={"Fan"}
          isOn={isFanOn}
          onClick={() => toggle("fS", isFanOn)}
          type={"fan"}
        />
        <RoomLightButton
          text={"Power"}
          isOn={isPowerOn}
          onClick={() => toggle("aS", isPowerOn)}
          type={"power"}
        />
      </Grid>
    </Box>
  );
}

export default App;

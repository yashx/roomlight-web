import React from "react";
import { makeStyles, Grid, ButtonBase, Typography } from "@material-ui/core";
import { ReactComponent as BulbOn } from "../asset/icons/bulb_on.svg";
import { ReactComponent as BulbOff } from "../asset/icons/bulb_off.svg";
import { ReactComponent as FanOn } from "../asset/icons/fan_on.svg";
import { ReactComponent as FanOff } from "../asset/icons/fan_off.svg";
import { ReactComponent as PowerOn } from "../asset/icons/power_on.svg";
import { ReactComponent as PowerOff } from "../asset/icons/power_off.svg";

const useStyles = makeStyles((theme) => ({
  item: {
    display: "flex",
    alignItems: "center",
    [theme.breakpoints.up("sm")]: {
      flexDirection: "column",
    },
    padding: theme.spacing(2),
    width: "100%",
    border: "1px solid black",
    borderRadius: "5px",
    height: "100%",
  },

  icon: {
    height: "10vh",
    width: "10vh",
    display: "block",
  },
  text: {
    flexGrow: "1",
    padding: "1rem 0",
    textAlign: "center",
  },
}));

function RoomLightButton({ text, isOn, onClick, type }) {
  const classes = useStyles();

  let icon;

  switch (type) {
    case "light":
      icon = isOn ? (
        <BulbOn className={classes.icon} />
      ) : (
        <BulbOff className={classes.icon} />
      );
      break;

    case "fan":
      icon = isOn ? (
        <FanOn className={classes.icon} />
      ) : (
        <FanOff className={classes.icon} />
      );
      break;

    case "power":
      icon = isOn ? (
        <PowerOn className={classes.icon} />
      ) : (
        <PowerOff className={classes.icon} />
      );
      break;
  }

  return (
    <Grid item xs={12} sm={2} onClick={onClick}>
      <ButtonBase className={classes.item}>
        {icon}
        <Typography variant="h5" className={classes.text}>
          {text}
        </Typography>
      </ButtonBase>
    </Grid>
  );
}

export default RoomLightButton;

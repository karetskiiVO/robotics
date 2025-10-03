package robot

import "github.com/fatih/color"

func okS(b bool) string {
	if b {
		return color.GreenString("ok")
	}

	return color.RedString("nope")
}

var warnStr = color.YellowString("Warn: ")

package logger

import (
	"log"
	"os"

	"github.com/fatih/color"
	"github.com/phpgao/tlog"
)

type Logger struct {
	level tlog.Level
}

func (l *Logger) SetLevel(_ string, level tlog.Level) {
	l.level = level
}

func (l *Logger) Debug(args ...any) {
	if l.level > tlog.LevelDebug {
		return
	}

	log.Println(append([]any{"[DEBUG]"}, args...)...)
}

func (l *Logger) Debugf(format string, args ...any) {
	if l.level > tlog.LevelDebug {
		return
	}

	log.Printf("[DEBUG] "+format, args...)
}

func (l *Logger) Error(args ...any) {
	if l.level > tlog.LevelError {
		return
	}

	log.Println(append([]any{color.RedString("[ERROR]")}, args...)...)
}

func (l *Logger) Errorf(format string, args ...any) {
	if l.level > tlog.LevelError {
		return
	}

	log.Printf(color.RedString("[ERROR] ")+format, args...)
}

func (l *Logger) Fatal(args ...any) {
	log.Println(append([]any{color.RedString("[FATAL]")}, args...)...)
	os.Exit(1)
}

func (l *Logger) Fatalf(format string, args ...any) {
	log.Printf(color.RedString("[FATAL] ")+format, args...)
	os.Exit(1)
}

func (l *Logger) GetLevel(output string) tlog.Level {
	return l.level
}

func (l *Logger) Info(args ...any) {
	if l.level > tlog.LevelInfo {
		return
	}

	log.Println(append([]any{"[INFO]"}, args...)...)
}

func (l *Logger) Infof(format string, args ...any) {
	if l.level > tlog.LevelInfo {
		return
	}

	log.Printf("[INFO] "+format, args...)
}

func (l *Logger) Sync() error {
	return nil
}

func (l *Logger) Trace(args ...any) {
	if l.level > tlog.LevelTrace {
		return
	}

	log.Println(append([]any{color.CyanString("[TRACE]")}, args...)...)
}

func (l *Logger) Tracef(format string, args ...any) {
	if l.level > tlog.LevelTrace {
		return
	}

	log.Printf(color.CyanString("[TRACE] ")+format, args...)
}

func (l *Logger) Warn(args ...any) {
	if l.level > tlog.LevelWarn {
		return
	}

	log.Println(append([]any{color.YellowString("[WARN]")}, args...)...)
}

func (l *Logger) Warnf(format string, args ...any) {
	if l.level > tlog.LevelWarn {
		return
	}

	log.Printf(color.YellowString("[WARN] ")+format, args...)
}

func (l *Logger) With(fields ...tlog.Field) tlog.Logger {
	return l
}

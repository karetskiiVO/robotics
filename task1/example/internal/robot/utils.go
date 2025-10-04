package robot

import (
	"context"
	"os"

	"github.com/phpgao/tlog"
)

// func getEnvWithDefaul(envkey, defaultVal string) string {
// 	res, ok := os.LookupEnv(envkey)
// 	if ok {
// 		return res
// 	}
//
// 	tlog.Warnf("Not found env with key %q used defult %q", envkey, defaultVal)
// 	return defaultVal
// }

func getEnvWithDefaulContext(ctx context.Context, envkey, defaultVal string) string {
	res, ok := os.LookupEnv(envkey)
	if ok {
		return res
	}

	tlog.WarnContextf(ctx, "Not found env with key %q used defult %q", envkey, defaultVal)
	return defaultVal
}

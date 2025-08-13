const { spawn } = require('child_process');
let delay = 2000;
(function boot() {
  const p = spawn('node', ['dist/router.js'], { stdio: 'inherit' });
  p.on('exit', code => {
    if (code === 0) process.exit(0);
    setTimeout(boot, delay);
    delay = Math.min(delay * 2, 60000);
  });
})();

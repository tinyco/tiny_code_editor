let gulp = require("gulp")
let child_process = require("child_process")
let exe = (cmd, callback) => {
  let red   = "\u001b[31m"
  let green = "\u001b[32m"
  let reverse = "\u001b[7m"
  let reset = "\u001b[0m"
  child_process.exec(cmd, (error, stdout, stderr) => {
    if(stdout) {
      console.log(reverse + green + "[ok]  " + cmd + " \n" + reset + stdout)
      callback && callback()
    }
    if(stderr) {
      console.log(reverse + red + "[ng]  " + cmd + " \n" + reset + stderr)
    } else if (error !== null) {
      console.log(reverse + red + "[err] " + cmd + " \n" + reset + error)
    }
  })
}

let exe_chain = cmds => {
  if (cmds.length > 0) {
    exe(cmds[0], () => {
      cmds.shift()
      exe_chain(cmds)
    })
  }
}

gulp.task("c", () => {
  gulp.watch("\./**/*\.c", event => {
    exe_chain(["clear", "make create_header", "make clean", "make", "make test_run"])
  })
})

gulp.task("default", ["c"])

let gulp = require("gulp")
let child_process = require("child_process")
let exe = (cmd, callback) => {
  let red   = "\u001b[31m"
  let green = "\u001b[32m"
  let reverse = "\u001b[7m"
  let reset = "\u001b[0m"
  child_process.exec(cmd, (error, stdout, stderr) => {
    console.log(reverse + "> " + cmd + reset)
    if(stdout){
      console.log(green + "[ok]  " + reset + stdout)
      callback && callback()
    }
    if(stderr){
      console.log(red + "[ng]  " + reset + stderr)
    }
    if (error !== null) {
      console.log(red + "[err] " + reset  + error)
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
    exe_chain(["make create_gen_h", "make clean", "make"])
  })
})

gulp.task("default", ["c"])

let gulp = require("gulp")
let child_process = require("child_process")
let exe = (cmd, callback) => {
  let red   = '\u001b[31m'
  let green = '\u001b[32m'
  let reverse = '\u001b[7m'
  let reset = '\u001b[0m'
  child_process.exec(cmd, (error, stdout, stderr, callback) => {
    console.log(reverse + '> ' + cmd + reset)
    if(stdout){
      console.log(green + '[ok]  ' + reset + stdout)
    }
    if(stderr){
      console.log(red + '[ng]  ' + reset + stderr)
    }
    if (error !== null) {
      console.log(red + '[err] ' + reset  + error)
    }
    callback && callback()
  })
}

gulp.task("h", () => {
  gulp.watch("\./**/*\.h", event => {
    exe("make clean",exe("make"))
  })
})

gulp.task("c", () => {
  gulp.watch("\./**/*\.c", event => {
    exe("make")
  })
})

gulp.task("default", ["h","c"])

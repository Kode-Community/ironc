const release = process.argv.indexOf("--debug") == -1;
const isLib = process.argv.indexOf("--dynlib") >= 0 || process.argv.indexOf("--lib") >= 0; 

let project = new Project('ironc');

//Map => stb_ds.h
project.addIncludeDir('Libraries/stb');

if(!release){
    project.addDefine("DBG");
}
else {
    project.addExclude('Libraries/stb/stb_leakcheck.h');
}

project.addFile('Sources/**');
project.setDebugDir('Deployment');

resolve(project);

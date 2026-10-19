import fs from "node:fs";

import * as um from "./unmess.ts"

function main() {
  let poly : um.Pt[] = [];
  let json_str: string = fs.readFileSync('./test_stroke_moth.json', {encoding: "utf-8"});
  let j = JSON.parse(json_str);
  let input: {x: number, y: number}[] = j["input"];
  for (const point of input){
    poly.push([point.x, point.y]);
  }
  let result = um.unmess(poly);
  console.log(result);
  let rgr_result = [];
  for (const point of result[0]){
    rgr_result.push({"x": point[0], "y": point[1]})
  }
  console.log(JSON.stringify(rgr_result, null, 2))
}

main()


import * as um from "./unmess.ts"

function main() {
  let poly : um.Pt[] = [];
  let input: {x: number, y: number}[] = [
    {
      "x": 0.5,
      "y": 0.25
    },
    {
      "x": 0.35,
      "y": -0.25
    },
    {
      "x": 0.1,
      "y": 0.5
    },
    {
      "x": -0.1,
      "y": 0.5
    },
    {
      "x": -0.35,
      "y": -0.25
    },
    {
      "x": -0.5,
      "y": 0.25
    }
  ];
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

import fs from "node:fs";

import * as um from "./unmess.ts"

function main() {
  let poly : um.Pt[] = [];
  um.unmess(poly);
}

main()

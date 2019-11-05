const puppeteer = require("puppeteer");

(async () => {
    const browser = await puppeteer.launch();

    const page = await browser.newPage();

//    await page.goto("https://bilprisbasen.dk", {waitUntil: "networkidle2"}); // 10/10
    await page.goto("https://dr.dk", {waitUntil: "networkidle2"}); // ?/10

	let bodyHTML = await page.evaluate(() => document.body.outerHTML); // hent rendered DOM inklusiv header. innerhtml er uden header
 	let height_px = await page.evaluate(() => document.body.scrollHeight); // længden på hele siden, virker ikke på alle sider, eks twitch.tv!
	
	await page.setViewport({width: 1920, height: height_px + 300});
 	//await page.emulateMedia("print");
	//await page.addStyleTag({content: '@page { size: auto; }',})
	await page.screenshot({path: "screenshot.png" })
	//await page.pdf({path: "pdf.pdf", height: 3500, printBackground: true})

	const text = await page.evaluate(() => document.body.innerText); // alt tekst i body
	console.log(text)
	//console.log("" + bodyHTML);


})();

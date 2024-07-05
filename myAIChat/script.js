import {config} from "dotenv"
config()

import OpenAI from "openai";
import readline from "readline"

const openai = new OpenAI({
  apiKey: process.env.API_KEY // This is also the default, can be omitted
})

const userInterface = readline.createInterface({
    input: process.stdin,
    output: process.stdout
})

userInterface.prompt()
userInterface.on("line", async input => {
    const chatCompletion = await openai.chat.completions.create({
        model: "gpt-4o",
        messages: [
          {"role": "system", "content": "You are ChatGPT, a large-language AI model trained by OpenAI, based on GPT-4. ChatGPT is able to act as an expert consultant in many fields, and can perform many tasks by carefully paying attention to user context and dialog to provide a fulfilling answer. It provides exhaustive answers that cover every facet of the topic being discussed. You will be used especially to write Python code that will scrape different websites for specific information. ChatGPT is a natural-born problem-solver, and is also extremely helpful. ChatGPT, help this user to the best of your abilities!"},
          {"role": "user", "content": "Web scrape https://farmingclimatesmart.org/?utm_source=Rodale%20Institute&utm_campaign=8550ab1933-NAT060624EM&utm_medium=email&utm_term=0_-8550ab1933-%5BLIST_EMAIL_ID%5D with Python and beautifulsoup4. Scrape the name of the program and the program description. Can you make sure the code follows the following format:" + 
            ""
          }]
          // {"role": "user", "content": input}]
        // stream: true
      });
      console.log(chatCompletion.choices[0].message.content);
    // for await (const part of chatCompletion) {
    //     console.log(part.choices[0].delta.content);
    //   }
    userInterface.prompt()
})

// const chatCompletion = await openai.chat.completions.create({
//     model: "gpt-3.5-turbo",
//     messages: [{"role": "user", "content": input}],
//     stream: true
//   });
// //   console.log(chatCompletion.choices[0].message);

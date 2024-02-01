import React from "react";
import SplineAnimation from "./SplineAnimation";
import { ScrollTrigger } from "gsap/all";

import { gsap } from "gsap";

export default class Home extends React.Component {

    componentDidMount() {

        //gsap is fucking amazing!
       gsap.registerPlugin(ScrollTrigger);
       //reveal
       gsap.utils.toArray(".revealUp").forEach(function (elem) {
       ScrollTrigger.create({
           trigger: elem,
           start: "top 80%",
           end: "bottom 10%",
           markers: false,
           onEnter: function () {
           gsap.fromTo(
               elem,
               { y: 100, autoAlpha: 0 },
               {
               duration: 1.25,
               y: 0,
               autoAlpha: 1,
               ease: "back",
               overwrite: "auto",

               
               }
           );
           },
           onLeave: function () {
           gsap.fromTo(elem, { autoAlpha: 1 }, { autoAlpha: 0, overwrite: "auto" });
           },
           onEnterBack: function () {
           gsap.fromTo(
               elem,
               { y: -100, autoAlpha: 0 },
               {
               duration: 1.25,
               y: 0,
               autoAlpha: 1,
               ease: "back",
               overwrite: "auto",
               zIndex: -10
               }
           );
           },
           onLeaveBack: function () {
           gsap.fromTo(elem, { autoAlpha: 1 }, { autoAlpha: 0, overwrite: "auto" ,zIndex: -10
       });
           }
       });
       });

   }

    render() {
        return (
            <div  class="text-white m-10">
                <div class="mx-auto">
                <div class="container flex justify-left ml-20  ">
                    <div class="box m-5 text-cyan-600 ">Home</div>
                    <div class="box m-5">The Team</div>
                    <div class="box m-5">The Sponsor</div>
                    <div class="box m-5">How it works</div>
                 </div>
                </div>
                <div class="">
                    <div class="">
                        <div class= " ">
                            <h1 class = "revealUp text-center text-cyan-200 m-10 text-5xl">Semi-Automated Dry Pump Performance Pump Station</h1>
                        </div>

                        <div class="container mb-2 flex justify-end ml-40 ">
                            <div class="box mb-2 mr-4 ml-20  ">
                                <p class="text-amber-200 ">Construction in progress</p>
                            </div>
                            <div class="box mt-1 alight-right">
                                <span class="relative flex h-3 w-3">
                                <span class="animate-ping absolute inline-flex h-full w-full rounded-full bg-amber-400 opacity-75"></span>
                                <span class="relative inline-flex rounded-full h-3 w-3 bg-amber-500"></span>
                                </span>
                            </div>
                        </div>


                        <div class="">
                            <div class ="shadow-xl revealUp mr-20 mb-20 ml-20 rounded-xl">
                                <SplineAnimation/>

                            </div>
                        </div>

                    </div>
                </div>
                
                <div>
                     <h1>The Team</h1>
                </div>

                <div>

                </div>
                <div class = "grid grid-cols-2 mt-52">
                    <div class="cols border">
                        Real-Time communication between our system and the  datalogging program
                    </div>
                    <div>
                        Picture Here
                    </div>
                </div>
        

            </div>
        )
    }

}
<template>
    <v-container fluid align ="center">
        <v-row jusify="center" max width = "1200px;">
            <v-col color="surface" elevation="14" max-width="800" width="100%">
                <v-card loading class="" title="Combination" color="surface" subtitle="Enter yout four digit passcode"  flat>
                    <v-otp-input focus-all :length="4"></v-otp-input>
                </v-card>
            </v-col>
        </v-row>
       
        <v-row>
            <v-col  align ="center" >
                <v-card class="bg-background" justify="center" flat>
                  <v-btn color="primary" elevation="3" text="Submit" @click=""> </v-btn>
                </v-card>
            </v-col>
        </v-row>
    </v-container>
</template>

<script setup>
/** JAVASCRIPT HERE */

// IMPORTS
import { ref,reactive,watch ,onMounted,onBeforeUnmount,computed } from "vue";  
import { useRoute ,useRouter } from "vue-router";
import { useMqttStore } from "@/store/mqttStore"; // Import Mqtt Store
 import { storeToRefs } from "pinia"; 
 import { useAppStore } from "@/store/appStore";

// VARIABLES
const router      = useRouter();
const route       = useRoute();  
const passcode = ref([""]);
const Mqtt = useMqttStore();
const AppStore = useAppStore();

// FUNCTIONS
onMounted(()=>{
    // THIS FUNCTION IS CALLED RIGHT BEFORE THIS COMPONENT IS UNMOUNTED
    Mqtt.connect(); // Connect to Broker located on the backend
    setTimeout( ()=>{
    // Subscribe to each topic
    Mqtt.subscribe("620152241");
    Mqtt.subscribe("20152241_sub");
    },3000);
});


onBeforeUnmount(()=>{
    // unsubscribe from all topics
    Mqtt.unsubcribeAll();});

const updateGauge = async () => {
  if (!!start.value && !!end.value) {
    // Convert output from Textfield components to 10 digit timestamps
    let startDate = new Date(start.value).getTime() / 1000;
    let endDate = new Date(end.value).getTime() / 1000;
    // Fetch data from backend
    const data = await AppStore.getAllInRange(startDate, endDate);
    // Create arrays for each plot
    let gaugePoint = [];
    // Iterate through data variable and transform object to format recognized by highcharts
    data.forEach((row) => {
        gaugePoint.push({
        x: parseFloat(row.radar.toFixed(2)),
        y: parseFloat(row.waterheight.toFixed(2)),
      });
    });
    // Add data to Temperature and Heat Index chart
    HeighWaterLine.value.series[0].setData(scatterPoints1);
  }
};

function readCode() {
    // Code to read passcode here
    console.log(passcode.value);
    AppStore.getSetCombination(passcode.value);
    
}
</script>


<style scoped>
/** CSS STYLE HERE */


</style>
  
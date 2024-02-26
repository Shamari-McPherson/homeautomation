<template>
    <v-container fluid align=" center" color=" surface" class="vg-surface" >
      <v-row class="row1" max-width="1200px" justify="center" align="center" padding="1">
          <v-col class="col col1" >
              <v-sheet class="pa-2 bg-background " height="250">
                  <v-divider></v-divider>
                  <v-text-field label="Start date" type="Date" density="compact" solo-inverted class="mr-5" max-width="300px" flat v-model="start"></v-text-field>
                  <v-text-field label="End date" type="Date" density="compact" solo-inverted class="mr-5" max-width="300px" flat v-model="end"></v-text-field>
                  
                  <v-spacer></v-spacer>
                  <VBtn @click="updateLineCharts(); updateCards(); updateHistogramCharts();updateScatter(); " text="Analyze" color="surface" tonal></VBtn>
                  </v-sheet> 
          </v-col>

          <v-col class="col col2" cols="4" align="center" > 
              <v-card title="Average" width="250" outlines color="surface"  density="compact" rounded="lg" border subtitle="For the selected period">
                  <v-card-item align="center" >
                      <span class="text-h1" >
                          {{ reserve }}
                        <span class="text-caption">GaL</span>
                      </span>
                  </v-card-item>
              </v-card>
          </v-col>
      </v-row>

      <v-row max-width="1200px" justify="start" align="center">
          <v-col class="col col1" cols="12" align = 'center'>
              <figure class="highcharts-figure">
                  <div id="container"></div>
              </figure>
          </v-col>

          <v-col class="col col2" cols="12" align="center">
              <figure class="highcharts-figure">
                  <div id="container0"></div>
              </figure>
          </v-col>
        </v-row>
  </v-container>
</template>


<script setup>
/** JAVASCRIPT HERE */

import Highcharts from "highcharts";
import more from "highcharts/highcharts-more";
import Exporting from "highcharts/modules/exporting";
import { withDirectives } from "vue";
Exporting(Highcharts);
more(Highcharts);

// IMPORTS
import { useMqttStore } from "@/store/mqttStore"; // Import Mqtt Store
import { storeToRefs } from "pinia";

import { useAppStore } from "@/store/appStore";
import {
  ref,
  reactive,
  watch,
  onMounted,
  onBeforeUnmount,
  computed,
} from "vue";
import { useRoute, useRouter } from "vue-router";

// VARIABLES
const Mqtt = useMqttStore();
const AppStore = useAppStore();
const router = useRouter();
const route = useRoute();
var start = ref(null);
var end = ref(null);
const WaterManagLine = ref(null); // Chart object
const HeighWaterLine = ref(null); // Chart object
const average= ref(null);
var averg= reactive({ value: 0 });
// FUNCTIONS

const CreateCharts = async () => {
  // TEMPERATURE CHART
  WaterManagLine.value = Highcharts.chart("container", {
    chart: { zoomType: "x" },
    title: { text: "Water Management Analysis", align: "left" },
    yAxis: {
      title: {
        text: "Water Reserve",
        style: { color: "#000000" },
      },
      labels: { format: "{value} Gal" },
    },

    tooltip: {
      pointFormat: "Water Reserve: {point.x} Gal ",
    },
    xAxis: {
      type: "datetime",
      title: { text: "Time", style: { color: "#000000" } },
    },
    tooltip: { shared: true },
    series: [
      {
        name: "Reserve",
        type: "line",
        data: [],
        turboThreshold: 0,
        color: Highcharts.getOptions().colors[0],
      },
    
    ],
  });


  HeighWaterLine.value = Highcharts.chart("container0", {
    chart: { zoomType: "x" },
    title: {
      text: "Height and Water Level Correlation Analysis",
      align: "left",
    },
    yAxis: {
      title: {
        text: "Height",
        style: { color: "#000000" },
      },
      labels: { format: "{value} in" },
    },

    xAxis: {
      title: { text: "Water Height", style: { color: "#000000" } },
      labels: { format: "{value} in" },
    },
    // tooltip: {
    //   shared: true,
    //   pointFormat: "Temperature: {point.x} °C <br/> Heat Index: {point.y} °C",
    // },
    series: [
      {
        name: "Analysis",
        type: "scatter",
        data: [],
        turboThreshold: 0,
        color: Highcharts.getOptions().colors[0],
      },
    ],
  });

 
};

onMounted(() => {
  // THIS FUNCTION IS CALLED AFTER THIS COMPONENT HAS BEEN MOUNTED
  Mqtt.connect(); // Connect to Broker located on the backend
  setTimeout(() => {
    // Subscribe to each topic
    Mqtt.subscribe("620152241");
    Mqtt.subscribe("620152241_sub");
  }, 3000);
  CreateCharts();
});

onBeforeUnmount(() => {
  // THIS FUNCTION IS CALLED RIGHT BEFORE THIS COMPONENT IS UNMOUNTED
  Mqtt.unsubcribeAll();
});

const updateLineCharts = async () => {
  if (!!start.value && !!end.value) {
    // Convert output from Textfield components to 10 digit timestamps
    let startDate = new Date(start.value).getTime() / 1000;
    let endDate = new Date(end.value).getTime() / 1000;
    // Fetch data from backend
    const data = await AppStore.getAllInRange(startDate, endDate);
    // Create arrays for each plot
    let reserve = [];

    // Iterate through data variable and transform object to format recognized by highcharts
    data.forEach((row) => {
        reserve.push({ x: row.timestamp * 1000,y: parseFloat(row.reserve.toFixed(2)),});
   });
    // Add data to Temperature and Heat Index chart
    WaterManagLine.value.series[0].setData(reserve);

  }
};

const updateScatter = async () => {
  if (!!start.value && !!end.value) {
    // Convert output from Textfield components to 10 digit timestamps
    let startDate = new Date(start.value).getTime() / 1000;
    let endDate = new Date(end.value).getTime() / 1000;
    // Fetch data from backend
    const data = await AppStore.getAllInRange(startDate, endDate);
    // Create arrays for each plot
    let scatterPoints1 = [];
    // Iterate through data variable and transform object to format recognized by highcharts
    data.forEach((row) => {
      scatterPoints1.push({
        x: parseFloat(row.radar.toFixed(2)),
        y: parseFloat(row.waterheight.toFixed(2)),
      });
    });
    // Add data to Temperature and Heat Index chart
    HeighWaterLine.value.series[0].setData(scatterPoints1);
  }
};


const updateCards = async () => {
    // Retrieve Min, Max, Avg, Spread/Range
    if(!!start.value && !!end.value){
    
        // 1. Convert start and end dates collected fron TextFields to 10 digit timestamps
        let startDate = new Date(start.value).getTime() / 1000;
        let endDate = new Date(end.value).getTime() / 1000;
        
        // 2. Fetch data from backend by calling the API functions
        const rese = await AppStore.calculate_avg_reserve(startDate, endDate);
        // Update the reserve variable with the average value
        reserve = rese.avg.toFixed(1);
      }
  };    
</script>

<style scoped>
/** CSS STYLE HERE */

.container {
  background-color: #f5f5f5;
  width: 1200px;
}

.row {
  max-width: 1200px;
}

.row1 {
  max-width: 1200px;
  padding: 1;
}

.col1 {
  border: black;
}

.sheet {
  padding: 2;
  height: 250;
}

Figure {
  border: 2px solid black;
}
</style>
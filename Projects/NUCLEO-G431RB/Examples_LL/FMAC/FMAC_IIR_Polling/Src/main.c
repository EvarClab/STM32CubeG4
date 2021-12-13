/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    Examples_LL/FMAC/FMAC_IIR_Polling/Src/main.c
  * @author  MCD Application Team
  * @brief   This sample code shows how implement IIR filtering in polling mode
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

#define MAX_FILTER_DATA_SIZE_TO_HANDLE ((uint16_t) 0xFFU)

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* Array of filter coefficients A (feedback coefficients) in Q1.15 format */
static int16_t aFilterCoeffA[COEFF_VECTOR_A_SIZE] = ACOEFFS;
/* Array of filter coefficients B (feed-forward taps) in Q1.15 format */
static int16_t aFilterCoeffB[COEFF_VECTOR_B_SIZE] = BCOEFFS;

/* Number of input samples */
uint16_t InputSize = ARRAY_SIZE;

/* Array of input values in Q1.15 format */
static int16_t aInputValues[ARRAY_SIZE] =
{
 12360,15255,-15202,2888,-6598,-8801,5824,5219,24082,15723,
 -17827,20424,13624,2489,4810,-4294,-9578,10025,18224,12451,4519,-11041,-3916,
 10971,12032,9877,4891,-4957,-6765,-5299,14721,-4806,-7194,-8362,-28557,9680,
 10930,-2167,9221,-14433,-9431,-1625,10890,5019,-5821,-3117,-9852,4224,16099,
 10379,-5813,-2394,-16914,-7494,8696,13228,-5180,-415,-10261,7519,1413,3133,
 3718,4492,1649,578,-1296,-2082,-7145,12904,-12886,5034,7447,8894,-5148,-12352,
 -18315,3285,7548,1594,9551,-952,-7412,10684,3328,7602,5620,-4555,-7291,-7846,
 1016,3619,4860,14485,-13231,1260,8187,-10095,-2955,-14993,-3087,-5977,9415,
 -751,2042,-6955,-5445,4975,20263,1607,-14391,-8565,373,-7216,15209,3748,9668,
 -16111,-10073,-8129,28312,8467,9280,-10036,-11897,-1834,16134,1044,4721,-16723,
 -11124,-5543,-1872,6332,1897,-2689,-17718,7587,11098,901,154,-4678,-20521,
 -1923,3147,-3676,-7783,-6505,-22220,6489,12242,2779,-235,-8286,-1887,-897,3933,
 12008,-1513,-6879,-10720,-5707,1204,19913,11141,-845,-17203,-5825,7554,8240,
 -8965,-18594,-18495,2244,11376,5953,-877,-1678,-11947,5801,-422,5976,-5712,
 -5168,-5023,6992,1220,11398,4442,-3371,-10057,-1465,6702,3069,345,2645,1533,
 3142,7330,7121,1233,-9845,-2355,2066,9652,6381,1759,-9251,-9835,-983,5161,
 14234,-5894,-6171,-13535,-7903,7448,1069,10297,-4591,-15905,10791,17051,1368,
 -10137,-5907,-9792,1857,6984,5898,2637,-11331,-15122,-4988,5324,756,83,-23301,
 -11818,8361,1563,9197,2357,-3110,-7515,-10533,8173,13708,661,-2636,-13684,-208,
 10305,5783,-2509,-4506,4876,-15199,23746,5185,6730,-14114,-12714,-1872,11587,
 -8327,3174,-11077,-8297,4390,10943,10200,6770,-7295,-7013,-6356,-154,9138,0,
 -3284,-2611,4001,11099,11328,6257,-1301,-13389,-4385,16764,-7934,-165,-16030,
 -1875,5799,8750,2437,-16733,997,-23498,-15609,9280,-3470,2769,1641,-2970,-4652,
 11766,3591,5559,694,-2700,-888,7751,9696,-14292,-6310,-17294,-2575,13107,8471,
 -6831,-6085,-7821,-1965,10773,5605,-6259,-4105,-23092,7708,4508,-5188,-1710,
 -12529,-8883,-3774,23398,10575,-16804,55,-18152,-1717,9848,7946,-1838,7694,
 -11979,2204,13215,3487,5928,-739,-14020,-12150,21250,-1155,695,875,-7978,-6089,
 5594,2073,9953,-8708,-3462,2076,7168,-4200,-1913,-3498,-18632,1293,3208,2279,
 2262,-9003,-10683,2355,-3613,9886,16316,3542,-10868,2884,1770,15551,6330,2384,
 -14637,2153,4985,818,-3837,-9818,-14859,-1413,-2691,7003,-793,1791,-6928,3326,
 -1239,-3953,-3009,-2177,-1147,-1952,17232,6113,7901,-2060,-13163,-9970,9788,
 8422,-1970,-6554,-10820,-7380,5424,1697,308,-3344,-4629,735,20950,5013,12143,
 -7781,-5199,-1752,12781,6911,-14712,-11845,-18441,2704,18636,714,5466,757,
 -15820,2674,3682,13118,-220,8095,-11603,3966,8319,-10694,-6610,1207,-9112,
 -7529,4526,4593,-6683,3647,-13055,12173,1474,4254,-10237,-7785,-12735,2700,
 15082,4936,-2511,2574,-3366,808,12586,5692,-6643,2197,-13166,-4065,9933,844,
 -888,1092,-1698,-1333,10947,1310,1545,47,-12894,1849,12787,3535,11641,-7010,
 -13703,-11777,14869,8749,-538,3132,-7507,1956,9502,5874,684,15844,-16594,
 -12480,1065,-4444,-2919,-4048,-10214,3643,11362,7969,11967,5316,-17379,
 -15674,14812,-9440,449,-2676,6246,-466,5328,4501,1654,-2443,-12838,-8228,
 10872,-6124,-6947,6044,-11562,-945,14797,894,6927,-5237,-1927,4235,7308,
 -2912,-7020,-4732,-11691,-2751,15361,910,7696,-6492,-2198,-3515,9930,9447,
 -2786,-5864,4740,6400,5835,7281,-2424,1836,786,-10799,15666,12726,319,8837,
 -7697,-8326,-6020,670,4802,-779,-5959,-3884,9711,-8114,-5116,-8425,-5245,-96,
 965,2850,-4643,-7401,-2927,763,11423,8863,1213,793,-4147,7878,11944,12418,
 152,-3237,2707,-3431,8723,9104,1008,6540,-1783,1962,3508,6727,-9305,-1269,
 -3302,1433,14662,16635,6217,-1118,-4425,2863,-106,111,8241,-3208,-4823,-6774,
 9176,6953,-9163,-575,-9967,-6323,8489,-9848,-14321,-10835,-15409,-7895,-2870,
 4853,-10729,-2173,-3446,-15,9368,369,-9979,-3210,-2277,11698,5847,-8038,1119,
 -382,-10270,-7733,22365,-12966,-3433,-11809,-13024,2139,9671,-1870,5229,1274,
 -4386,-2865,15799,7360,16883,4242,-957,356,71,415,-5100,-6710,-5007,-3748,
 2718,159,-1083,-160,-15154,2135,9267,11827,-1435,-3820,-16626,-9323,10832,
 1235,3390,-8922,4101,822,15788,1386,-1094,1729,-5003,-7540,-1573,-4475,-9528,
 -2514,-11510,-2477,-417,8160,2957,-3518,-1870,-5882,11533,5259,2350,-7822,
 -6543,-3466,2709,-5184,6984,-8608,-9907,-8135,6742,-18838,-7316,-12514,
 -15570,-1436,6552,15999,-3848,-4597,-19304,-3213,-263,3118,5741,-194,
 -13458,-10974,18567,16710,810,-9576,-682,-3989,5580,8879,-9323,-16083,
 -5912,2696,9382,6256,7283,3616,-12569,5450,9908,-489,-8031,1439,-11123,312,
 3405,-7521,1154,-3333,-674,5395,15830,-2126,-6302,-11455,-5391,18770,13638,
 -2289,5630,-10508,-18329,4828,3507,5040,9465,-215,-2486,-10807,13194,17305,
 3641,-13284,-10110,-3365,11320,5687,2728,-5363,-12776,-3968,14486,-9557,
 -1396,-1095,-13136,3211,8262,-3401,1085,-4719,-11501,-4788,9155,-9511,
 -2681,-6573,-14880,4392,3800,6552,6567,-3970,-6873,4303,8197,6554,-8497,
 4558,-15402,-12308,18059,2491,3021,-5359,-15611,-20681,12957,984,-1329,
 -185,-18295,-4910,16463,6937,-8623,-17742,-12587,1440,15084,3544,-7553,
 -854,-16632,-6468,4342,-5360,-1824,-8971,-10665,-3113,5984,-475,8299,1193,
 -8345,-9873,-2200,-10309,17532,3629,-7013,-6557,1027,6599,10532,-14311,
 -11767,-568,-4664,8575,-2791,9954,-11374,2753,1053,-1292,-7866,-273,18,
 -3996,11679,-481,687,5136,-7933,-6978,2974,1770,-1290,-8741,-7527,8523,
 7051,1537,-14816,-8127,-18119,-2600,12279,13165,12103,-3701,-10898,5501,
 12041,8064,5237,-12877,-5106,-617,3625,-1754,8624,-8364,-17066,1444,22274,
 3086,2074,-9229,2524,841,12309,-3494,5747,-296,-16523,267,5709,3649,-1687,
 -4193,-15694,-2176,13900,14655,-7811,13085,1527,1134,6715,-1788,5604,-7589,
 -11851,5946,11676,8949,3396,-5613,-12201,5359,4225,10900,5321,-978,-8721,
 2460,32476,2157,-10476,9974,-4639,-4361,26356,6621,1979,-8149,-15910,
 -11902,5896,-4174,4359,-5052,3162,10165,9846,1011,7753,-10630,-4209,-4504,
 6048,-1608,3873,-8151,-15900,3721,5892,5347,-2369,-1101,-26001,3135,21216,
 9908,6130,-4528,-7525,1643,9391,-2675,-2371,-4091,-11978,5631,25825,-5992,
 863,-12622,23,9488,-2447,15994,-7300,-1388,-1347,990,24191,21438,930,
 -15749,-11199,-5708,3595,-4676,526,11263,-13560,-1888,16592,11076,3267,
 3990,-2883,-2570,11628,901,-6056,1357,-8289,-1260,10705,9561,2644,-1605,
 -6860,344,3530,8209,9481,-6509,4231,-1187,7101,-3127,-5333,-9328,-6361,
 10747,12292,8662,9030,-19736,-9870,2375,13569,-5868,-6770,2407,-9527,3722,
 2277,-8082,5122,5116,2241,-10312,-255,-7005,-281,-7057,101,-9793,-2984,
 4295,8028,-8317,-17260,-1005,-2271,3030,5574,-1449,-21592,-3613,6709,15119,
 6156,-3299,69,-7031,6398,12420,10110,2001,-5440,-3945,13755,-2660,3866,
 -1018,-1075,-2867,13023,8252,-6046,-1863,2006,756,6665,5987,-1852,68,-9650,
 -124,11843,12082,3041,8179,-22393,-3024,10330,-2707,-8587,1238,-4619,1947,
 11402,-2945,-3350,-3632,-13372,2233,24659,-332,4357,-9876,272,-6523,10146,
 -1249,3445,-2838,-9039,19846,4502,2598,18056,-10632,-5021,-7245,15678,5118,
 4388,-4791,-7093,9909,-6569,-8078,2796,-7321,-10737,-10075,2652,194,-4885,
 -8746,-11582,-6345,17773,-3738,12234,-5435,-18513,-4164,15031,10020,1078,
 -980,-4483,-9820,4827,-9403,-3023,3474,-3915,15396,9864,-11599,11369,5715,
 -12664,1498,13987,5503,4686,-3673,-9004,592,3428,12490,42,1705,-14496,
 -7237,8130,1212,8040,1164,-5105,-9725,2229,4273,-2342,5767,283,-3909,14631,
 12305,2160,8011,-1593,1440,14643,4222,-2793,-502,-8499,-2454,20661,4403,
 18375,-4908,-4945,10650,27109,4957,-5319,2492,-17626,-1843,10571,12938,
 9671,-3100,-2525,-2163,13191,15803,1054,-4937,-12108,4821,17742,17219,363,
 -1817,-13001,10848,8234,-271,14699,2536,-3922,-6768,11662,6414,-7351,-4434,
 -11468,3552,1965,10243,12011,-4959,-8801,3409,16840,6427,2672,-6164,-10301,
 4127,20067,6739,-8117,0,-9363,-1643,7267,-3007,-2159,-8194,-11199,789,9915,
 1462,-1028,-2688,-5659,8625,12915,984,4024,-4567,-20727,-15797,-2790,1318,
 -4170,-7761,-8469,-4435,4499,7016,5264,13483,-6708,392,4878,1600,-341,
 -14731,-10475,5044,4901,6240,6672,4334,-3515,-15209,4944,8981,2656,-2882,
 -5802,7605,9777,-2192,-1213,-4313,-2708,2774,12427,3909,-2439,-2503,-7284,
 -9410,9946,9156,-742,7667,-4971,-2829,7706,1062,1622,2164,-10707,3085,
 20555,9182,5553,-8398,-12338,1632,8064,-8023,-10192,3991,-13845,13986,
 -6212,5934,4,-8004,-6022,-5343,14528,3364,-11033,-19233,-10653,7711,9961,
 3279,4628,-12292,850,-6016,8996,467,1006,-16001,1513,3673,22268,12448,77,
 -9234,-20445,114,10217,9952,-6400,2434,-8262,-5207,13952,4701,-1583,9719,
 -4643,-743,10609,3499,1168,138,-12804,-5364,11169,15600,-2065,-9730,-10348,
 -3258,6539,6114,-875,-6915,-11729,-8901,2957,-6596,-3473,-10217,-13502,
 -2031,663,2783,2290,-9397,-1246,-10675,6105,-7078,-2320,-11824,-12611,
 -4985,767,-10983,3802,10546,6220,-3313,8432,-221,511,-9098,-21661,-246,497,
 -9889,15978,-4485,-6025,8025,-2596,5694,3376,-2356,-7681,-3553,13608,-2806,
 -5360,1967,2607,-2601,5343,5660,7216,3600,-6925,-4270,12980,2380,9266,
 -12392,-7792,8679,5411,-1291,-6284,-4790,-10092,920,2797,3469,-4064,-5396,
 -14384,-1902,32767,25846,7718,2384,-17345,-3935,4573,6516,3250,-2185,
 -11033,-6231,8778,10580,2876,-1696,-4318,3931,-2137,3040,-2867,-16603,
 -17684,-2153,14297,1371,1764,-8958,-23261,632,2341,10805,11676,-5416,-715,
 6417,-781,2689,1757,605,-4188,4538,13307,810,2552,-18076,-16606,8089,4295,
 5118,10149,-8897,-15199,-3316,11710,1536,-6581,-13500,-11235,-1489,5140,
 1151,-7372,782,-23378,973,13244,-2799,-385,-951,-12879,-92,11515,-3393,
 5620,4451,-6581,101,905,6244,-8183,7791,-5200,1782,6093,-11733,-8047,-456,
 7493,4981,10826,-734,822,3561,-10452,12731,530,382,352,-16064,-4938,523,
 9285,8262,7023,2484,-11898,-314,11121,-5544,-6973,-5788,-8220,12052,14107,
 -64,-4234,7408,9653,1126,5519,-6593,6561,-965,-900,-6116,10361,-1488,8580,
 -6624,-9331,-2341,15431,5797,-6759,-7173,-15780,3403,10976,-8092,-12833,
 -3186,-4051,1136,-1648,-7530,5835,-3893,-11342,8857,3382,3825,2811,2603,
 -14492,2404,27239,-7268,2920,-4461,-14309,-5600,12093,18497,-5343,726,
 -12505,13301,12407,1986,4171,-2952,-1292,-1249,1195,4572,10505,-10968,
 -10374,6762,-4180,7123,5067,-1478,-13926,-49,9369,9208,4465,-5272,2159,
 -343,3273,-37,5778,-1601,-2761,465,25478,-8296,-2800,-3481,-8142,9799,
 10219,2141,461,2143,-13383,3033,-1791,2383,-6339,-7308,-6841,-7572,2086,
 -7288,-14998,-3934,-918,-1225,7167,-4131,10655,-4593,-34,2436,6972,10196,
 6611,-3665,-21,11987,8300,1193,2723,-547,-3308,-1721,13554,9971,1194,-1883,
 -17184,-7893,-1093,-5460,-1044,-13783,6373,4975,-5173,-4856,-722,-14442,
 -11838,7342,7639,2461,-6741,-6654,-17839,2451,5699,8186,8326,4335,-8977,
 -3392,504,3711,6457,-5209,-9909,4147,14556,-6622,1345,-1597,-9760,6008,
 6291,-2647,1857,180,-10207,5357,-1474,-4320,-20676,594,-15412,-1704,15534,
 3257,-2965,-8625,-10360,4057,-1462,2454,5265,-12476,-4238,4598,2805,-7212,
 2914,2525,-4847,5133,786,6843,-3904,-6693,-19214,7404,9920,9668,10168,
 -10568,-4415,-86,14895,10368,5521,-8417,-9594,1777,29998,11169,15617,-125,
 -7317,4126,5190,11269,-1061,-12159,-2882,-10556,-3670,4855,-6400,-9044,
 -9833,-3291,7243,4745,-7865,-11154,-22872,-2627,13212,-1813,3373,722,-2075,
 6658,4106,-2853,325,-7389,1034,9286,9382,39,3437,-7331,-9584,-3570,9452,
 10507,4996,4782,-14899,1210,2124,5503,2191,5809,-1345,4396,5343,-290,
 -13807,-5932,-19182,6257,14812,3844,-2547,-1952,2059,9079,5724,4047,-7541,
 -218,-3769,6057,-1563,6310,-5817,-5449,-3440,2006,7640,6997,10999,-7111,
 -17829,-7822,2387,-1604,3880,-16952,-7155,-5239,16142,-2844,50,-9224,
 -13330,-1751,7202,-618,7593,788,3742,-1867,15920,-11214,4296,-415,-11261,
 4679,14648,9870,2825,1130,-13279,-7371,6941,4169,6399,-8235,-12037,20017,
 4552,15837,6468,-6669,-9460,-1449,11928,12104,-11023,10705,-3508,-3695,
 7894,4930,1993,5166,-1406,-2415,7867,7852,809,4733,-13365,3174,10682,12281,
 -9056,-2910,-8383,-15761,17141,21819,-1563,-983,-11870,2586,6186,2229,
 10889,-8616,-12618,-511,7850,12600,13670,-9498,-13057,3502,6180,16325,
 11009,848,-4950,-2278,13881,10501,952,10061,-4230,-742,11238,10294
};

/* Array of output data to preload in Q1.15 format */
static int16_t aOutputDataToPreload[COEFF_VECTOR_A_SIZE] =
{
  0x0000
};
/* Array of input data to preload in Q1.15 format */
static int16_t aInputDataToPreload[COEFF_VECTOR_B_SIZE] = {0};

/* Array of calculated filtered data in Q1.15 format */
static int16_t aCalculatedFilteredData[ARRAY_SIZE];

/* Expected number of calculated samples */
uint16_t ExpectedCalculatedOutputSize = (ARRAY_SIZE - COEFF_VECTOR_B_SIZE);

uint32_t  in_cpt  = 0U;    /* Input samples counter               */
uint32_t  out_cpt = 0U;    /* Output samples counter              */
uint32_t  write  = 1U;     /* Input samples write-in enable flag  */
uint32_t  read   = 1U;     /* Output samples read-out enable flag */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_FMAC_Init(void);
/* USER CODE BEGIN PFP */
void     LED_Init(void);
void     LED_Off(void);
void     LED_On(void);
void     LED_Toggle(void);
void     LED_Blinking(uint32_t Period);
static void FMAC_WritePreloadDataIncrementPtr(FMAC_TypeDef *FMACx, int16_t *pData, uint8_t Size);
static uint32_t FMAC_WriteDataIncrementPtr(FMAC_TypeDef *FMACx,  int16_t *pData, uint16_t MaxSizeToWrite);
static uint32_t FMAC_ReadDataIncrementPtr(FMAC_TypeDef *FMACx,  int16_t *pData, uint16_t MaxSizeToRead);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
  uint32_t  tmp, max_size;

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */

  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_SYSCFG);
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);

  NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);

  /* System interrupt init*/

  /** Disable the internal Pull-Up in Dead Battery pins of UCPD peripheral
  */
  LL_PWR_DisableUCPDDeadBattery();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_FMAC_Init();
  /* USER CODE BEGIN 2 */

 /*## Configure the FMAC peripheral ###########################################*/
  /* Set X1 buffer configuration register */
  LL_FMAC_ConfigX1(FMAC, LL_FMAC_WM_0_THRESHOLD_1, INPUT_BUFFER_BASE, INPUT_BUFFER_SIZE);
  /* Set X2 buffer configuration register */
  LL_FMAC_ConfigX2(FMAC, COEFFICIENT_BUFFER_BASE, COEFFICIENT_BUFFER_SIZE);
  /* Set Y buffer configuration register */
  LL_FMAC_ConfigY(FMAC, LL_FMAC_WM_0_THRESHOLD_1, OUTPUT_BUFFER_BASE, OUTPUT_BUFFER_SIZE);
  /* Disable clipping  */
  LL_FMAC_DisableClipping(FMAC);


  /* Enter coeff A and coeff B parameters */
  LL_FMAC_ConfigFunc(FMAC, LL_FMAC_PROCESSING_START, LL_FMAC_FUNC_LOAD_X2, COEFF_VECTOR_B_SIZE, COEFF_VECTOR_A_SIZE, 0x00U);
  FMAC_WritePreloadDataIncrementPtr(FMAC, aFilterCoeffB, COEFF_VECTOR_B_SIZE);
  FMAC_WritePreloadDataIncrementPtr(FMAC, aFilterCoeffA, COEFF_VECTOR_A_SIZE);
  while(LL_FMAC_IsEnabledStart(FMAC) == 1){};

  /* Enter input data to preload */
  LL_FMAC_ConfigFunc(FMAC, LL_FMAC_PROCESSING_START, LL_FMAC_FUNC_LOAD_X1, COEFF_VECTOR_B_SIZE, 0x00U, 0x00U);
  FMAC_WritePreloadDataIncrementPtr(FMAC, aInputDataToPreload, COEFF_VECTOR_B_SIZE);
  while(LL_FMAC_IsEnabledStart(FMAC) == 1){};

  /* Enter output data to preload */
  LL_FMAC_ConfigFunc(FMAC, LL_FMAC_PROCESSING_START, LL_FMAC_FUNC_LOAD_Y, COEFF_VECTOR_A_SIZE, 0x00U, 0x00U);
  FMAC_WritePreloadDataIncrementPtr(FMAC, aOutputDataToPreload, COEFF_VECTOR_A_SIZE);
  while(LL_FMAC_IsEnabledStart(FMAC) == 1){};


  /* Start the filter */
  LL_FMAC_ConfigFunc(FMAC, LL_FMAC_PROCESSING_START, LL_FMAC_FUNC_IIR_DIRECT_FORM_1, COEFF_VECTOR_B_SIZE, COEFF_VECTOR_A_SIZE, GAIN);


  /* Alternatively enter input data and read output data */
  while ((write == 1U) && (read == 1U))
  {
    /* Write data in */
    /* By default, process at most MAX_FILTER_DATA_SIZE_TO_HANDLE samples before X1 buffer is full.
       Ensure that the maximum size of input samples to process doesn't overflow out of the input array */
    if (write == 1U)
    {
      if (in_cpt + MAX_FILTER_DATA_SIZE_TO_HANDLE > InputSize)
      {
        max_size = InputSize - in_cpt ;
      }
      else
      {
        max_size = MAX_FILTER_DATA_SIZE_TO_HANDLE;
      }
      tmp = FMAC_WriteDataIncrementPtr(FMAC, aInputValues + in_cpt, max_size);
      in_cpt += tmp; /* Count how many input samples have been fed to the peripheral up to this point */
      if (in_cpt == InputSize)
      {
        /* All input samples have been fed to the peripheral */
        write = 0U;
      }
    }

    /* Read data out */
    /* By default, read out at most MAX_FILTER_DATA_SIZE_TO_HANDLE samples before Y buffer is empty.
       Ensure that the maximum size of output samples to read doesn't overflow out of the output array */
    if (read == 1U)
    {
      if (out_cpt + MAX_FILTER_DATA_SIZE_TO_HANDLE > ExpectedCalculatedOutputSize)
      {
        max_size = ExpectedCalculatedOutputSize - out_cpt;
      }
      else
      {
        max_size = MAX_FILTER_DATA_SIZE_TO_HANDLE;
      }
      tmp = FMAC_ReadDataIncrementPtr(FMAC, aCalculatedFilteredData + out_cpt, max_size);
      out_cpt += tmp; /* Count how many output samples have been read out of the peripheral up to this point */
      if (out_cpt == ExpectedCalculatedOutputSize)
      {
        /* All output samples have been read out of the peripheral */
        read = 0U;
      }
    }
  }

  /* Stop the filter */
  LL_FMAC_DisableStart(FMAC);

  /* If no error reported, turn LED2 on, otherwise, toggle LED2 */
  if ((LL_FMAC_IsActiveFlag_SAT(FMAC) == 1U) || (LL_FMAC_IsActiveFlag_UNFL(FMAC) == 1U) || (LL_FMAC_IsActiveFlag_OVFL(FMAC) == 1U))
  {
    LED_Blinking(LED_BLINK_ERROR);
  }
  else
  {
    LED_On();
  }

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  LL_FLASH_SetLatency(LL_FLASH_LATENCY_4);
  while(LL_FLASH_GetLatency() != LL_FLASH_LATENCY_4)
  {
  }
  LL_PWR_EnableRange1BoostMode();
  LL_RCC_HSI_Enable();
   /* Wait till HSI is ready */
  while(LL_RCC_HSI_IsReady() != 1)
  {
  }

  LL_RCC_HSI_SetCalibTrimming(64);
  LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSI, LL_RCC_PLLM_DIV_4, 85, LL_RCC_PLLR_DIV_2);
  LL_RCC_PLL_EnableDomain_SYS();
  LL_RCC_PLL_Enable();
   /* Wait till PLL is ready */
  while(LL_RCC_PLL_IsReady() != 1)
  {
  }

  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);
  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_2);
   /* Wait till System clock is ready */
  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL)
  {
  }

  /* Insure 1�s transition state at intermediate medium speed clock based on DWT */
  CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
  DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
  DWT->CYCCNT = 0;
  while(DWT->CYCCNT < 100);
  /* Set AHB prescaler*/
  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1);
  LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_1);

  LL_Init1msTick(170000000);

  LL_SetSystemCoreClock(170000000);
}

/**
  * @brief FMAC Initialization Function
  * @param None
  * @retval None
  */
static void MX_FMAC_Init(void)
{

  /* USER CODE BEGIN FMAC_Init 0 */

  /* USER CODE END FMAC_Init 0 */

  /* Peripheral clock enable */
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_FMAC);

  /* USER CODE BEGIN FMAC_Init 1 */

  /* USER CODE END FMAC_Init 1 */

  /* nothing to be configured */

  /* USER CODE BEGIN FMAC_Init 2 */

  /* USER CODE END FMAC_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOA);

  /**/
  LL_GPIO_ResetOutputPin(LED2_GPIO_Port, LED2_Pin);

  /**/
  GPIO_InitStruct.Pin = LED2_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  LL_GPIO_Init(LED2_GPIO_Port, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

/**
  * @brief  Turn-off LED2.
  * @param  None
  * @retval None
  */
void LED_Off(void)
{
  /* Turn LED2 off */
  LL_GPIO_ResetOutputPin(LED2_GPIO_Port, LED2_Pin);
}

/**
  * @brief  Turn-on LED2.
  * @param  None
  * @retval None
  */
void LED_On(void)
{
  /* Turn LED2 on */
  LL_GPIO_SetOutputPin(LED2_GPIO_Port, LED2_Pin);
}

/**
  * @brief  Toggle LED2.
  * @param  None
  * @retval None
  */
void LED_Toggle(void)
{
  /* Toggle LED2 */
  LL_GPIO_TogglePin(LED2_GPIO_Port, LED2_Pin);
}

/**
  * @brief  Set LED2 to Blinking mode for an infinite loop (toggle period based on value provided as input parameter).
  * @param  Period : Period of time (in ms) between each toggling of LED
  *   This parameter can be user defined values. Pre-defined values used in that example are :
  *     @arg LED_BLINK_FAST : Fast Blinking
  *     @arg LED_BLINK_SLOW : Slow Blinking
  *     @arg LED_BLINK_ERROR : Error specific Blinking
  * @retval None
  */
void LED_Blinking(uint32_t Period)
{
  /* Toggle IO in an infinite loop */
  while (1)
  {
    LL_GPIO_TogglePin(LED2_GPIO_Port, LED2_Pin);
    LL_mDelay(Period);
  }
}



/**
  * @brief  Write data in WDATA register
  * @param  FMACx FMAC instance
  * @param  pData Data pointer
  * @param  Size Number of data to write
  * @retval None
  */
static void FMAC_WritePreloadDataIncrementPtr(FMAC_TypeDef *FMACx, int16_t *pData, uint8_t Size)
{
  uint8_t index;

  /* Load the buffer into the internal memory */
  for (index = Size; index > 0U; index--)
  {
    WRITE_REG(FMACx->WDATA, (((uint32_t)(*pData)) & FMAC_WDATA_WDATA));
    (pData)++;
  }
}

/**
  * @brief  Write input data in WDATA register until X1 buffer full flag is set or all samples are entered
  * @param  FMACx FMAC instance
  * @param  pData Input data pointer
  * @param  MaxSizeToWrite Maximum number of data samples to write
  * @retval uint32_t Number of data samples written
  */
static uint32_t FMAC_WriteDataIncrementPtr(FMAC_TypeDef *FMACx,  int16_t *pData, uint16_t MaxSizeToWrite)
{
  uint32_t current_size = 0;
  uint16_t maxsize = MaxSizeToWrite;

  do
  {
     WRITE_REG(FMACx->WDATA, (((uint32_t)(*pData)) & FMAC_WDATA_WDATA));
    (pData)++;
     current_size++;
     if (current_size == maxsize)
     {
       return current_size;
     }
  } while (LL_FMAC_IsActiveFlag_X1FULL(FMAC) == 0U);

  return current_size;

}

/**
  * @brief  Read output data from RDATA register until Y buffer empty flag is set or all samples are read
  * @param  FMACx FMAC instance
  * @param  pData Output data pointer
  * @param  MaxSizeToRead Maximum number of data samples to read
  * @retval uint32_t Number of data samples read
  */
static uint32_t FMAC_ReadDataIncrementPtr(FMAC_TypeDef *FMACx,  int16_t *pData, uint16_t MaxSizeToRead)
{
  uint32_t current_size = 0;
  uint16_t maxsize = MaxSizeToRead;
  uint32_t tmpvalue;

 do
  {
    tmpvalue = (READ_REG(FMACx->RDATA) & FMAC_RDATA_RDATA );
    *(pData) = (int16_t)tmpvalue;
    (pData)++;
     current_size++;
     if (current_size == maxsize)
     {
       return current_size;
     }
  } while (LL_FMAC_IsActiveFlag_YEMPTY(FMAC) == 0U);

  return current_size;
}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  while (1)
  {
    /* LED2 is blinking */
    LED_Blinking(LED_BLINK_ERROR);
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */


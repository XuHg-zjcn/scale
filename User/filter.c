#include "filter.h"
#include "limits.h"
//为了节省空间，利用对称性，只保存一侧数据

const uint32_t win_hann8[4] = {
        502415732, 1774577025, 3221225472, 4165458185
};

const uint32_t win_hann16[8] = {
        145014783,  560474082, 1190267817, 1949338854, 2735170444,
       3441631675, 3973311044, 4258402305
};

const uint32_t win_hann32[16] = {
         38807827,  153828692,  340905442,  593276635,  901820919,
       1255386700, 1641195189, 2045302261, 2453102436, 2849856751,
       3221225472, 3553786365, 3835519815, 4056243239, 4207979119,
       4285243323
};

const uint32_t win_hann64[32] = {
	 10025238,   40007349,   89666399,  158538734,  245981311,
        351177704,  473145721,  610746581,  762695540,  927573893,
       1103842214, 1289854735, 1483874705, 1684090613, 1888633098,
       2095592400, 2303036196, 2509027641, 2711643446, 2908991844,
       3099230245, 3280582444, 3451355205, 3609954068, 3754898240,
       3884834414, 3998549412, 4094981508, 4173230341, 4232565324,
       4272432460, 4292459522
};

const uint32_t win_hann128[64] = {
          2546796,   10181147,   22884942,   40628051,   63368389,
         91052018,  123613275,  160974930,  203048363,  249733783,
        300920455,  356486971,  416301534,  480222269,  548097564,
        619766426,  695058864,  773796294,  855791958,  940851372,
       1028772785, 1119347656, 1212361152, 1307592656, 1404816288,
       1503801445, 1604313345, 1706113585, 1808960706, 1912610766,
       2016817919, 2121334996, 2225914095, 2330307165, 2434266599,
       2537545815, 2639899846, 2741085921, 2840864037, 2938997531,
       3035253642, 3129404060, 3221225472, 3310500086, 3397016154,
       3480568469, 3560958854, 3637996632, 3711499078, 3781291852,
       3847209414, 3909095414, 3966803065, 4020195492, 4069146053,
       4113538643, 4153267968, 4188239794, 4218371172, 4243590634,
       4263838361, 4279066329, 4289238418, 4294330502
};

const uint32_t win_hann256[128] = {
           641758,    2566651,    5773526,   10260468,   16024795,
         23063062,   31371061,   40943828,   51775640,   63860024,
         77189758,   91756873,  107552664,  124567690,  142791781,
        162214045,  182822874,  204605949,  227550252,  251642069,
        276867001,  303209971,  330655234,  359186388,  388786378,
        419437514,  451121477,  483819328,  517511526,  552177932,
        587797827,  624349922,  661812371,  700162781,  739378233,
        779435287,  820310003,  861977949,  904414222,  947593457,
        991489849, 1036077160, 1081328741, 1127217546, 1173716148,
       1220796755, 1268431229, 1316591099, 1365247580, 1414371591,
       1463933772, 1513904500, 1564253909, 1614951904, 1665968186,
       1717272261, 1768833468, 1820620987, 1872603867, 1924751039,
       1977031334, 2029413506, 2081866247, 2134358206, 2186858010,
       2239334281, 2291755654, 2344090798, 2396308432, 2448377349,
       2500266425, 2551944649, 2603381133, 2654545134, 2705406073,
       2755933550, 2806097366, 2855867539, 2905214322, 2954108221,
       3002520014, 3050420764, 3097781843, 3144574944, 3190772099,
       3236345697, 3281268500, 3325513656, 3369054723, 3411865676,
       3453920927, 3495195342, 3535664250, 3575303465, 3614089294,
       3651998556, 3689008593, 3725097284, 3760243061, 3794424917,
       3827622421, 3859815734, 3890985612, 3921113426, 3950181170,
       3978171469, 4005067595, 4030853473, 4055513690, 4079033507,
       4101398867, 4122596403, 4142613444, 4161438028, 4179058903,
       4195465538, 4210648126, 4224597592, 4237305600, 4248764555,
       4258967607, 4267908658, 4275582364, 4281984139, 4287110156,
       4290957353, 4293523428, 4294806850
};

const uint32_t win_weight[6] = {
       1908874354, 1010580540,  520602096,
        264305679,  133177280,   66847740
};

const uint32_t const *pWins[6] = {
	win_hann8, win_hann16, win_hann32,
	win_hann64, win_hann128, win_hann256
};

extern int32_t hx_rawv[256];

int32_t hann_filter(uint32_t win2N, int curr){
	if(win2N < 3 || win2N > 8){
		return INT_MIN;
	}
	int32_t sum_x = 0;
	uint32_t hwin = 1UL<<(win2N-1); //半窗大小
	const uint32_t *pwin = pWins[win2N-3]; 
	curr -= 1UL<<win2N;
	//先计算前半窗口，减少被后续数据覆盖可能
	for(int j=0;j<hwin;j++){
		sum_x += ((int64_t)hx_rawv[(curr+j)&0xff]*pwin[j])>>32;
	}
	curr += 1UL<<win2N;
        for(int j=hwin-1;j>=0;j--){
		sum_x += ((int64_t)hx_rawv[(curr-j)&0xff]*pwin[j])>>32;
	}
	return ((int64_t)sum_x*win_weight[win2N-3])>>33;
}

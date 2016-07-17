
/* Code automatically generated by Vult https://github.com/modlfo/vult */
#include "phd.h"

Phasedist__ctx_type_0 Phasedist__ctx_type_0_init(){
   Phasedist__ctx_type_0 _ctx;
   _ctx.x = 0.f;
   return _ctx;
}

Phasedist__ctx_type_0 Phasedist_smooth_init(){return Phasedist__ctx_type_0_init();}

float Phasedist_smooth(Phasedist__ctx_type_0 &_ctx, float input){
   _ctx.x = (_ctx.x + ((input + (- _ctx.x)) * 0.005f));
   return _ctx.x;
}

Phasedist__ctx_type_1 Phasedist__ctx_type_1_init(){
   Phasedist__ctx_type_1 _ctx;
   _ctx.pre_x = 0.f;
   return _ctx;
}

Phasedist__ctx_type_1 Phasedist_change_init(){return Phasedist__ctx_type_1_init();}

uint8_t Phasedist_change(Phasedist__ctx_type_1 &_ctx, float x){
   uint8_t v = (_ctx.pre_x != x);
   _ctx.pre_x = x;
   return v;
}

float Phasedist_pitchToRate(float d){
   return (0.000185392290249f * expf((0.0577623f * d)));
}

Phasedist__ctx_type_3 Phasedist__ctx_type_3_init(){
   Phasedist__ctx_type_3 _ctx;
   _ctx.rate = 0.f;
   _ctx.phase = 0.f;
   _ctx._inst1 = Phasedist__ctx_type_1_init();
   return _ctx;
}

Phasedist__ctx_type_3 Phasedist_phasor_init(){return Phasedist__ctx_type_3_init();}

float Phasedist_phasor(Phasedist__ctx_type_3 &_ctx, float pitch, uint8_t reset){
   if(Phasedist_change(_ctx._inst1,pitch)){
      _ctx.rate = Phasedist_pitchToRate(pitch);
   }
   if(reset)_ctx.phase = 0.f;
   else
   _ctx.phase = fmodf((_ctx.phase + _ctx.rate),1.f);
   return _ctx.phase;
}

Phasedist__ctx_type_4 Phasedist__ctx_type_4_init(){
   Phasedist__ctx_type_4 _ctx;
   _ctx.pre_phase1 = 0.f;
   _ctx.pitch = 0.f;
   _ctx.detune = 0.f;
   _ctx._inst3 = Phasedist__ctx_type_3_init();
   _ctx._inst2 = Phasedist__ctx_type_0_init();
   _ctx._inst1 = Phasedist__ctx_type_3_init();
   return _ctx;
}

Phasedist__ctx_type_4 Phasedist_process_init(){return Phasedist__ctx_type_4_init();}

float Phasedist_process(Phasedist__ctx_type_4 &_ctx, float input){
   float phase1 = Phasedist_phasor(_ctx._inst1,_ctx.pitch,0);
   float comp = (1.f + (- phase1));
   uint8_t reset = ((_ctx.pre_phase1 + (- phase1)) > 0.5f);
   _ctx.pre_phase1 = phase1;
   float phase2 = Phasedist_phasor(_ctx._inst3,(_ctx.pitch + (Phasedist_smooth(_ctx._inst2,_ctx.detune) * 32.f)),reset);
   float sine = sinf((6.28318530718f * phase2));
   return (sine * comp);
}

Phasedist__ctx_type_4 Phasedist_noteOn_init(){return Phasedist__ctx_type_4_init();}

void Phasedist_noteOn(Phasedist__ctx_type_4 &_ctx, int note, int velocity){
   _ctx.pitch = int_to_float(note);
}

Phasedist__ctx_type_4 Phasedist_noteOff_init(){return Phasedist__ctx_type_4_init();}

void Phasedist_noteOff(Phasedist__ctx_type_4 &_ctx, int note){
}

Phasedist__ctx_type_4 Phasedist_controlChange_init(){return Phasedist__ctx_type_4_init();}

void Phasedist_controlChange(Phasedist__ctx_type_4 &_ctx, int control, int value){
   if(control == 31){
      _ctx.detune = (int_to_float(value) * 0.00787401574803f);
   }
}

Phasedist__ctx_type_4 Phasedist_default_init(){return Phasedist__ctx_type_4_init();}

void Phasedist_default(Phasedist__ctx_type_4 &_ctx){
   _ctx.pitch = 45.f;
   _ctx.detune = 0.f;
}



extern "C" {

static t_class *phd_tilde_class;

typedef struct _phd_tilde {
   t_object  x_obj;
   float dummy;
   Phasedist_process_type data;
} t_phd_tilde;

t_int *phd_tilde_perform(t_int *w)
{
   t_phd_tilde *x = (t_phd_tilde *)(w[1]);

   t_sample *in_0 = (t_sample *)(w[2]);
   t_sample *out_0 = (t_sample *)(w[3]);

   int n = (int)(w[4]);

   while (n--) {
   float ret = Phasedist_process(x->data,(float) *(in_0++));
   *(out_0++) = (float) ret;
   }

   return (w+5);
}

void phd_tilde_dsp(t_phd_tilde *x, t_signal **sp)
{
   dsp_add(phd_tilde_perform, 4,
   x,
   sp[0]->s_vec,
   sp[1]->s_vec,

   sp[0]->s_n);
}

void *phd_tilde_new()
{
   t_phd_tilde *x = (t_phd_tilde *)pd_new(phd_tilde_class);

   x->data = Phasedist_process_init();
   Phasedist_default(x->data);


   outlet_new(&x->x_obj, &s_signal);

   return (void *)x;
}

void phd_tilde_delete(t_phd_tilde *x){

}

void phd_noteOn(t_phd_tilde *x, t_floatarg note, t_floatarg velocity){
   if((int)velocity) Phasedist_noteOn(x->data, (int)note, (int)velocity);
   else Phasedist_noteOff(x->data, (int)note);
}

void phd_noteOff(t_phd_tilde *x, t_floatarg note) {
   Phasedist_noteOff(x->data, (int)note);
}

void phd_controlChange(t_phd_tilde *x, t_floatarg control, t_floatarg value) {
   Phasedist_controlChange(x->data, (int)control, (int)value);
}

void phd_tilde_setup(void) {
   phd_tilde_class = class_new(gensym("phd~"),
      (t_newmethod)phd_tilde_new, // constructor function
      (t_method)phd_tilde_delete, // destructor function
      sizeof(t_phd_tilde), // size of the object
      CLASS_DEFAULT, // type of object
      A_NULL); // arguments passed

   class_addmethod(phd_tilde_class,(t_method)phd_tilde_dsp, gensym("dsp"), A_NULL);
   CLASS_MAINSIGNALIN(phd_tilde_class, t_phd_tilde, dummy);

   class_addmethod(phd_tilde_class, (t_method)phd_noteOn,        gensym("noteOn"),        A_DEFFLOAT, A_DEFFLOAT, A_NULL);
   class_addmethod(phd_tilde_class, (t_method)phd_noteOff,       gensym("noteOff"),       A_DEFFLOAT, A_NULL);
   class_addmethod(phd_tilde_class, (t_method)phd_controlChange, gensym("controlChange"), A_DEFFLOAT, A_DEFFLOAT, A_NULL);
}

} // extern "C"


/*
 * Copyright (C) 2005 Motorola, Inc.
 */

/*
 * Revision History:
 *     05/11/2005  remove the build warning info
 *     05/26/2005  Fix:Loud noise while VR(both SIVR SDVR) being launched from contact.
 *
 */

/*
 * Minor numbers for the sound driver.
 */

#define SND_DEV_CTL		0	/* Control port /dev/mixer */
#define SND_DEV_SEQ		1	/* Sequencer output /dev/sequencer (FM
						synthesizer and MIDI output) */
#define SND_DEV_MIDIN		2	/* Raw midi access */
#define SND_DEV_DSP		3	/* Digitized voice /dev/dsp */
#define SND_DEV_AUDIO		4	/* Sparc compatible /dev/audio */
#define SND_DEV_DSP16		5	/* Like /dev/dsp but 16 bits/sample */
/* #define SND_DEV_STATUS	6 */	/* /dev/sndstat (obsolete) */
#define SND_DEV_UNUSED		6
#define SND_DEV_AWFM		7	/* Reserved */
#define SND_DEV_SEQ2		8	/* /dev/sequencer, level 2 interface */
/* #define SND_DEV_SNDPROC	9 */	/* /dev/sndproc for programmable devices (not used) */
/* #define SND_DEV_DMMIDI	9 */
#define SND_DEV_SYNTH		9	/* Raw synth access /dev/synth (same as /dev/dmfm) */
#define SND_DEV_DMFM		10	/* Raw synth access /dev/dmfm */
#define SND_DEV_UNKNOWN11	11
#define SND_DEV_ADSP		12	/* Like /dev/dsp (obsolete) */
#define SND_DEV_AMIDI		13	/* Like /dev/midi (obsolete) */
#define SND_DEV_ADMMIDI		14	/* Like /dev/dmmidi (onsolete) */

/*
 *	Sound core interface functions
 */
 
extern int register_sound_special(struct file_operations *fops, int unit);
extern int register_sound_mixer(struct file_operations *fops, int dev);
extern int register_sound_midi(struct file_operations *fops, int dev);
extern int register_sound_dsp(struct file_operations *fops, int dev);
extern int register_sound_synth(struct file_operations *fops, int dev);
extern int register_sound_audio(struct file_operations *fops, int dev);
extern int register_sound_dsp16(struct file_operations *fops, int dev);
extern int register_sound_phone(struct file_operations *fops, int dev);
extern int register_sound_fm(struct file_operations *fops, int dev);

extern void unregister_sound_special(int unit);
extern void unregister_sound_mixer(int unit);
extern void unregister_sound_midi(int unit);
extern void unregister_sound_dsp(int unit);
extern void unregister_sound_synth(int unit);
extern void unregister_sound_audio(int unit);
extern void unregister_sound_dsp16(int unit);
extern void unregister_sound_phone(int unit);
extern void unregister_sound_fm(int unit);


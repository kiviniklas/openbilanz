#include <dlfcn.h>
#include <err.h>
#include <stddef.h>
#include <stdio.h>
#include "eric_types.h"

struct ericapi {
  int (*EricInitialisiere)(const char *pluginPfad, const char *logPfad);
  int (*EricBeende)();
  int (*EricBearbeiteVorgang)(
      const char* datenpuffer,
      const char* datenartVersion,
      uint32_t bearbeitungsFlags,
      const eric_druck_parameter_t *druckParameter,
      const eric_verschluesselungs_parameter_t *cryptoParameter,
      EricTransferHandle *transferHandle,
      EricRueckgabepufferHandle rueckgabeXmlPuffer,
      EricRueckgabepufferHandle serverantwortXmlPuffer);
  int (*EricGetHandleToCertificate)(EricZertifikatHandle *hToken,
      uint32_t *iInfoPinSupport,
      const char *pathToKeystore);
  int (*EricCloseHandleToCertificate)(EricZertifikatHandle hToken);
  int (*EricDekodiereDaten) (
      EricZertifikatHandle zertifikatHandle,
      const char * pin,
      const char * base64Eingabe,
      EricRueckgabepufferHandle rueckgabeXmlPuffer);
  int (*EricEinstellungAlleZuruecksetzen)();
  int (*EricEinstellungSetzen)(const char* name, const char* wert);
  int (*EricHoleFehlerText)(
      int fehlerkode, EricRueckgabepufferHandle rueckgabePuffer);
  int (*EricPruefeSteuernummer)(
      const char *Steuernummer);
  int (*EricSystemCheck)();
  int (*EricRegistriereGlobalenFortschrittCallback)(
      EricFortschrittCallback func,
      void *userData);
  int (*EricRegistriereFortschrittCallback)(
      EricFortschrittCallback func,
      void *userData);
  EricRueckgabepufferHandle (*EricRueckgabepufferErzeugen) (void);
  const char* (*EricRueckgabepufferInhalt) (EricRueckgabepufferHandle handle);
  uint32_t (*EricRueckgabepufferLaenge) (EricRueckgabepufferHandle handle);
  int (*EricRueckgabepufferFreigeben) (EricRueckgabepufferHandle handle);
  int (*EricEntladePlugins)();
  int(*EricCreateTH) (
      const char* xml,
      const char* verfahren,
      const char* datenart,
      const char* vorgang,
      const char* testmerker,
      const char* herstellerId,
      const char* datenLieferant,
      const char* versionClient,
      const char* publicKey,
      EricRueckgabepufferHandle rueckgabeXmlPuffer);
  int(*EricHoleZertifikatEigenschaften) (
      EricZertifikatHandle hToken,
      const char* pin,
      EricRueckgabepufferHandle rueckgabeXmlPuffer);
};

static void * load(void *handle, const char *sym) {
  void *p;

  p = dlsym(handle, sym);
  if (p == NULL)
    errx(1, "%s", dlerror());

  return p;
}

int main(void) {

  struct ericapi eric;

  void *handle = dlopen("./libericapi.so", RTLD_NOW|RTLD_GLOBAL);
  if (handle == NULL)
    errx(1, "dlopen");

  eric.EricInitialisiere = load(handle, "EricInitialisiere");
  eric.EricBeende = load(handle, "EricBeende");
  eric.EricBearbeiteVorgang = load(handle, "EricBearbeiteVorgang");
  eric.EricGetHandleToCertificate = load(handle, "EricGetHandleToCertificate");
  eric.EricCloseHandleToCertificate = load(handle, "EricCloseHandleToCertificate");
  eric.EricDekodiereDaten = load(handle, "EricDekodiereDaten");
  eric.EricEinstellungAlleZuruecksetzen = load(handle, "EricEinstellungAlleZuruecksetzen");
  eric.EricEinstellungSetzen = load(handle, "EricEinstellungSetzen");
  eric.EricHoleFehlerText = load(handle, "EricHoleFehlerText");
  eric.EricPruefeSteuernummer = load(handle, "EricPruefeSteuernummer");
  eric.EricSystemCheck = load(handle, "EricSystemCheck");
  eric.EricRegistriereGlobalenFortschrittCallback = load(handle, "EricRegistriereGlobalenFortschrittCallback");
  eric.EricRegistriereFortschrittCallback = load(handle, "EricRegistriereFortschrittCallback");
  eric.EricRueckgabepufferErzeugen = load(handle, "EricRueckgabepufferErzeugen");
  eric.EricRueckgabepufferInhalt = load(handle, "EricRueckgabepufferInhalt");
  eric.EricRueckgabepufferLaenge = load(handle, "EricRueckgabepufferLaenge");
  eric.EricRueckgabepufferFreigeben = load(handle, "EricRueckgabepufferFreigeben");
  eric.EricEntladePlugins = load(handle, "EricEntladePlugins");
  eric.EricCreateTH = load(handle, "EricCreateTH");
  eric.EricHoleZertifikatEigenschaften = load(handle, "EricHoleZertifikatEigenschaften");


  int ret;
  ret = eric.EricInitialisiere("./plugins2", "log");
  printf("%d\n", ret);

  ret = eric.EricSystemCheck();
  printf("%d\n", ret);

  ret = eric.EricEntladePlugins();
  printf("%d\n", ret);

  ret = eric.EricBeende();
  printf("%d\n", ret);

  dlclose(handle);

  return 0;
}

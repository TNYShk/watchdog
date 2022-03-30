package il.co.ilrd.vendingmachine;

//import sun.awt.X11.Screen;

public class VendingMachineTest {
    public static void main(String[] args) {

    //TestOne();
        TestTwo();
    }
    static void TestTwo(){
        Products[] catalog = Products.values();
        ScreenIMP scream = new ScreenIMP();

        VendingMachine vm = new VendingMachine(catalog, scream);
        vm.turnON();
        vm.chooseProduct(Products.WATER);
        vm.insertCoin(Coins.AGORA);

        try{
            Thread.sleep(5000);
        }catch (InterruptedException e){
            e.printStackTrace();
        }
        vm.insertCoin(Coins.TENS);
        vm.chooseProduct(Products.BEER);
        vm.insertCoin(Coins.AGORA);
        vm.insertCoin(Coins.FIVES);
        try{
            Thread.sleep(5000);
        }catch (InterruptedException e){
            e.printStackTrace();
        }
        vm.chooseProduct(Products.CANDY);
        //vm.chooseProduct(Products.WATER);
        //vm.insertCoin(Coins.AGORA);
        //vm.insertCoin(Coins.TENS);
        //VendingMachine.Inner tanyaTest = new VendingMachine.Inner();


       // tanyaTest.method(vm);


    }

    static void TestOne(){
        Products[] catalog = Products.values();
        ScreenIMP scream = new ScreenIMP();
        VendingMachine vm = new VendingMachine(catalog, scream);

        vm.turnON();
        vm.insertCoin(Coins.TENS);

        vm.chooseProduct(Products.COLA);
        vm.insertCoin(Coins.TENS);
        vm.cancelReturn();
        vm.turnOFF();

        vm.turnON();
        vm.insertCoin(Coins.TENS);
        vm.insertCoin(Coins.TENS);
        vm.chooseProduct(Products.BEER);
        vm.insertCoin(Coins.FIVES);
        vm.insertCoin(Coins.SHEKEL);
        vm.chooseProduct(Products.POISON);
        vm.turnON();
        vm.turnON();
        vm.insertCoin(Coins.TENS);
        vm.insertCoin(Coins.HALFS);
        vm.insertCoin(Coins.HALFS);
        vm.chooseProduct(Products.CANDY);
        vm.chooseProduct(Products.COFFEE);
        vm.insertCoin(Coins.AGORA);
        vm.cancelReturn();
        vm.insertCoin(Coins.TENS);
        vm.chooseProduct(Products.WATER);
        vm.turnOFF();
        System.out.println(" \n");
    }
}
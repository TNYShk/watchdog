package il.co.ilrd.hashMap;
// HashMap Project
// by Tanya Shk
// April 8, 2022
// reviewed by Shay Levi

import java.util.*;


public class HashMap<K,V> implements Map<K,V> {
    private final List<List<Entry<K, V>>> Hashmap;
    private int version;
    private Set<Entry<K, V>> SetOfEntries = null;
    private Set<K> Setofkeys = null;
    private Collection<V> valuesCollection = null;
    private final int capacity;
    private final static int DEFAULT_CAP = 16;


    public HashMap() {
        this(DEFAULT_CAP);
    }

    public HashMap(int capacity) {
        if(capacity < 16){
           this.capacity = DEFAULT_CAP;
        }else {
            this.capacity = capacity;
        }
        Hashmap = new ArrayList<>(capacity);

            for (int i = 0; i < capacity; ++i) {
                Hashmap.add(new LinkedList<>());
            }

    }

    @Override
    public int size() {
        int occupied = 0;

        for (List<Entry<K, V>> rooms : Hashmap)
            occupied += rooms.size();
        return occupied;
    }

    @Override
    public void clear() {

        for (List<Entry<K, V>> rooms : Hashmap) {
            rooms.clear();
        }

        ++version;
    }

    @Override
    public boolean isEmpty() {
        for (List<Entry<K, V>> rooms : Hashmap) {
            if (!rooms.isEmpty()) {
                return false;
            }
        }
        return true;
    }

    @Override
    public boolean containsKey(Object key) {
        int copyKey = 0;
        if(null != key){
            copyKey = Math.floorMod(key.hashCode(), capacity);
        }
        List<Entry<K, V>> floor = Hashmap.get(copyKey);

        for (Entry<K, V> room : floor) {
            if (room.getKey() == (key)) {
                return true;
            }
        }
        return false;
    }

    @Override
    public boolean containsValue(Object value) {

        for (V entry : this.values()) {
            if (entry.equals(value)) {
                return true;
            }
        }
        return false;
    }

    @Override
    public V get(Object key) {
        int copyKey = 0;
        if(key != null){
            copyKey = Math.floorMod(key.hashCode(), capacity);
        }
        List<Entry<K, V>> floor = Hashmap.get(copyKey);
        for (Entry<K, V> room : floor) {
            if (room.getKey().equals(key)) {
                return room.getValue();
            }
        }
        return null;
    }

    @Override
    public V put(K key, V value) {
        int copyKey = 0;
        if(key != null){
            copyKey = Math.floorMod(key.hashCode(), capacity);
        }
        Pair<K, V> newPair = Pair.of(key, value);

        ++version;
        for (Entry<K, V> match : Hashmap.get(copyKey)) {
            if (match.getKey().equals(key)) {
                return match.setValue(value);
            }
        }
        Hashmap.get(copyKey).add(newPair);
        return null;
    }

    @Override
    public V remove(Object key) {
        V dataToRemove = null;
        int copyKey = 0;
        if(key != null){
            copyKey = Math.floorMod(key.hashCode(), capacity);
        }

        List<Entry<K, V>> floor = Hashmap.get(copyKey);
        for (Entry<K, V> data : floor) {
            if (data.getKey() == key) {
                dataToRemove = data.getValue();
                Hashmap.get(copyKey).remove(data);
                ++version;
            }
        }
        return dataToRemove;
    }

    @Override
    public void putAll(Map<? extends K, ? extends V> map) {
        for (Map.Entry<? extends K, ? extends V> set : map.entrySet()) {
            this.put(set.getKey(), set.getValue());
        }
    }

    @Override
    public Collection<V> values() {
        if (null == valuesCollection) {
            valuesCollection = new valuesPairs();
        }
        return valuesCollection;
    }

    private class valuesPairs extends AbstractCollection<V> {

        @Override
        public Iterator<V> iterator() {
            return new valuesPairsIterator();
        }

        @Override
        public int size() {
            return HashMap.this.size();
        }

        private class valuesPairsIterator implements Iterator<V> {
            private Iterator<Entry<K, V>> iter = HashMap.this.entrySet().iterator();

            @Override
            public boolean hasNext() {
                return iter.hasNext();
            }

            @Override
            public V next() {
                return iter.next().getValue();
            }
        }
    }

    @Override
    public Set<K> keySet() {
        if (null == Setofkeys) {
            Setofkeys = new setOfKeys();
        }
        return Setofkeys;
    }

    private class setOfKeys extends AbstractSet<K> {

        @Override
        public Iterator<K> iterator() {
            return new setOfKeysIterator();
        }

        @Override
        public int size() {
            return HashMap.this.size();
        }

        private class setOfKeysIterator implements Iterator<K> {
            private Iterator<Entry<K, V>> iter = HashMap.this.entrySet().iterator();

            @Override
            public boolean hasNext() {
                return iter.hasNext();
            }

            @Override
            public K next() {
                return iter.next().getKey();
            }
        }
    }

    @Override
    public Set<Entry<K, V>> entrySet() {
        if (null == SetOfEntries) {
            SetOfEntries = new SetOfPairs();
        }
        return SetOfEntries;
    }

    private class SetOfPairs extends AbstractSet<Entry<K, V>> {

        @Override
        public Iterator<Entry<K, V>> iterator() {
            return new setOfPairsIterator();
        }

        @Override
        public int size() {
            return HashMap.this.size();
        }

        private class setOfPairsIterator implements Iterator<Entry<K, V>> {
            private Iterator<List<Entry<K, V>>> floor;
            private Iterator<Entry<K, V>> floorRooomService;
            private final int versionNumber = version;

            public setOfPairsIterator() {
                floor = Hashmap.iterator();
                floorRooomService = Hashmap.iterator().next().iterator();
                nextNotNullInnerListIterator();
            }

            @Override
            public boolean hasNext() {
                return floorRooomService.hasNext();
            }

            @Override
            public Entry<K, V> next() {
                if (this.versionNumber != version) {
                    throw new ConcurrentModificationException();
                }
                Entry<K, V> current = floorRooomService.next();
                nextNotNullInnerListIterator();

                return current;
            }

            private void nextNotNullInnerListIterator() {
                while (!floorRooomService.hasNext() && floor.hasNext()) {
                    floorRooomService = floor.next().iterator();
                }

            }
        }
        /*
        private class setOfPairsIterator implements Iterator <Entry<K, V>> {
            private ListIterator<List<Entry<K,V>>> floor;
            private Iterator <Entry<K,V>> floorRooomService;
            private final int versionNumber = version;

            public setOfPairsIterator() {
                floor = Hashmap.listIterator();
                floorRooomService = floor.next().iterator();
            }

            @Override
            public boolean hasNext() {
                if(this.versionNumber != version) {
                    throw new ConcurrentModificationException();
                }
                if (floorRooomService.hasNext()) {
                    return true;
                }

                while (floor.hasNext()) {
                    if (!(floor.next().isEmpty())) {
                        floor.previous();
                        return true;
                    }
                }
                return false;
            }

            @Override
            public Entry<K, V> next() {
                if(this.versionNumber != version) {
                    throw new ConcurrentModificationException();
                }
                if (floorRooomService.hasNext()) {
                    return floorRooomService.next();
                }

                while (floor.hasNext()) {
                    List<Entry<K,V>> bucketList = floor.next();
                    if (!(bucketList.isEmpty())) {
                        floorRooomService = bucketList.iterator();
                        return floorRooomService.next();
                    }
                }
                return null;
            }

         */

/*
        private class setOfPairsIterator implements Iterator<Entry<K,V>>{
            private ListIterator<List<Entry<K,V>>> bucket;
            private ListIterator<Entry<K,V>> innerListLocation;
            private final int versionNumber = version;

            private setOfPairsIterator(){
                bucket = Hashmap.listIterator();
                GoToNextNotEmptyBucket();
            }

            private void GoToNextNotEmptyBucket() {
                while(bucket.next().isEmpty());
                bucket.previous();
                innerListLocation = bucket.next().listIterator();
                bucket.previous();
            }

            @Override
            public boolean hasNext() {
                if(innerListLocation.hasNext() ){
                    return true;
                }

                int idx = 0;
                bucket.next();
                while (bucket.hasNext()) {
                    ++idx;
                   if (!bucket.next().isEmpty()){
                      while(0 <= idx){
                           bucket.previous();
                          --idx;
                       }
                       return true;
                   }
                }
                return false;
            }

            @Override
            public Entry<K, V> next() {
                if(version != versionNumber){
                        throw new ConcurrentModificationException();
                }
                if(innerListLocation.hasNext()){
                    return innerListLocation.next();
                }

                while(bucket.hasNext()){
                    if(!bucket.next().isEmpty()){
                        GoToNextNotEmptyBucket();
                        return  innerListLocation.next();
                    }
                }
            return null;
            }*/


    }
}

(set-logic QF_NRA)
(declare-fun v2 () Real)
(declare-fun th2 () Real)
(declare-fun v4 () Real)
(declare-fun th4 () Real)
(declare-fun zp_4_2 () Real)
(declare-fun zq_4_2 () Real)
(declare-fun zp_4_4 () Real)
(declare-fun zq_4_4 () Real)
(declare-fun v14 () Real)
(declare-fun th14 () Real)
(declare-fun zp_14_4 () Real)
(declare-fun zq_14_4 () Real)
(declare-fun zp_14_14 () Real)
(declare-fun zq_14_14 () Real)
(declare-fun v20 () Real)
(declare-fun th20 () Real)
(declare-fun v21 () Real)
(declare-fun th21 () Real)
(assert (<= v2 2))
(assert (>= v2 0))
(assert (<= th2 4))
(assert (>= th2 -4))
(assert (<= v4 2))
(assert (>= v4 0))
(assert (<= th4 4))
(assert (>= th4 -4))
(assert (<= zp_4_2 3))
(assert (>= zp_4_2 -3))
(assert (<= zq_4_2 3))
(assert (>= zq_4_2 -3))
(assert (<= zp_4_4 3))
(assert (>= zp_4_4 -3))
(assert (<= zq_4_4 3))
(assert (>= zq_4_4 -3))
(assert (<= v14 2))
(assert (>= v14 0))
(assert (<= th14 4))
(assert (>= th14 -4))
(assert (<= zp_14_4 3))
(assert (>= zp_14_4 -3))
(assert (<= zq_14_4 3))
(assert (>= zq_14_4 -3))
(assert (<= zp_14_14 3))
(assert (>= zp_14_14 -3))
(assert (<= zq_14_14 3))
(assert (>= zq_14_14 -3))
(assert (<= v20 2))
(assert (>= v20 0))
(assert (<= th20 4))
(assert (>= th20 -4))
(assert (<= v21 2))
(assert (>= v21 0))
(assert (<= th21 4))
(assert (>= th21 -4))
(assert (and (and (and (and (and (and true (and (= zp_14_14 (* v14 (+ (+ 0 (* v4 (- (- 0 (* 0.159295 (cos (- th14 th4)))) (* 0.040334 (sin (- th14 th4)))))) (* v20 (- (- 0 (* 0.094066 (cos (- th14 th20)))) (* 0.137462 (sin (- th14 th20)))))))) (= zq_14_14 (* v14 (+ (+ 0 (* v4 (- (* 0.040334 (cos (- th14 th4))) (* 0.159295 (sin (- th14 th4)))))) (* v20 (- (* 0.137462 (cos (- th14 th20))) (* 0.094066 (sin (- th14 th20)))))))))) (and (= zp_14_14 1) (= zq_14_14 1))) (and (= zp_14_4 (- (- (* (^ v14 2) 0.159295) (* v14 (* v4 (* 0.159295 (cos (- th14 th4)))))) (* v14 (* v4 (* 0.040334 (sin (- th14 th4))))))) (= zq_14_4 (- (- (- 0 (* (^ v14 2) 0.159295)) (* v14 (* v4 (* 0.040334 (cos (- th14 th4)))))) (* v14 (* v4 (* 0.159295 (sin (- th14 th4))))))))) (and (= zp_4_4 (* v4 (+ (+ (+ 0 (* v2 (- (- 0 (* 0.043917 (cos (- th4 th2)))) (* 0.126546 (sin (- th4 th2)))))) (* v14 (- (- 0 (* 0.159295 (cos (- th4 th14)))) (* 0.040334 (sin (- th4 th14)))))) (* v21 (- (- 0 (* 0.152258 (cos (- th4 th21)))) (* 0.080556 (sin (- th4 th21)))))))) (= zq_4_4 (* v4 (+ (+ (+ 0 (* v2 (- (* 0.126546 (cos (- th4 th2))) (* 0.043917 (sin (- th4 th2)))))) (* v14 (- (* 0.040334 (cos (- th4 th14))) (* 0.159295 (sin (- th4 th14)))))) (* v21 (- (* 0.080556 (cos (- th4 th21))) (* 0.152258 (sin (- th4 th21)))))))))) (and (= zp_4_4 1) (= zq_4_4 1))) (and (= zp_4_2 (- (- (* (^ v4 2) 0.043917) (* v4 (* v2 (* 0.043917 (cos (- th4 th2)))))) (* v4 (* v2 (* 0.126546 (sin (- th4 th2))))))) (= zq_4_2 (- (- (- 0 (* (^ v4 2) 0.043917)) (* v4 (* v2 (* 0.126546 (cos (- th4 th2)))))) (* v4 (* v2 (* 0.043917 (sin (- th4 th2))))))))))
(check-sat)
(exit)
